#include "ClientEventController.hpp"

#include <sys/socket.h>
#include <unistd.h>

#include <iostream>

#include "EventController.hpp"
#include "StartProcessor.hpp"

// constructor / destructor
ClientEventController::ClientEventController(int clientSocket)
    : config_(NULL), processor_(NULL) {
  fd_ = clientSocket;
  processor_ = new StartProcessor(*this);
  ProcessResult processResult = nextProcessor();
  while (processResult.nextProcessor_) {
    processResult = nextProcessor();
  }
  if (processResult.error_) {
    std::cout << "error: fatal close";
    clear(true);
    throw std::logic_error("fatal error");
  }
}

ClientEventController::~ClientEventController() { delete processor_; }

void ClientEventController::addEventController(
    int socket, const std::vector<ServerConfig *> &configs) {
  ClientEventController *client = new ClientEventController(socket);
  client->setServerConfigs(configs);
  Multiplexer::getInstance().addReadEvent(socket, client);
  Multiplexer::getInstance().addTimeoutEvent(socket, client);
}

// IClient
const Request &ClientEventController::getRequest() const { return request_; }

void ClientEventController::setRequest(const Request &request) {
  request_ = request;
}

const Response &ClientEventController::getResponse() const { return response_; }

void ClientEventController::setResponse(const Response &response) {
  response_ = response;
}

void ClientEventController::setResponseStatusCode(int code) {
  response_.setStatusCode(code);
}

void ClientEventController::setResponseHeader(const std::string &key,
                                              const std::string &value) {
  response_.setHeader(key, value);
}

DataStream &ClientEventController::getDataStream() { return stream_; }

const std::vector<char> &ClientEventController::getRecvBuffer() const {
  return recvBuffer_;
}

static ServerConfig *selectServerConfig(
    Request request, std::vector<ServerConfig *> serverConfigs) {
  if (request.hasHeader("Host") == false) {
    return NULL;
  }
  std::string host = request.getHeader("Host");
  host = host.substr(0, host.find(":"));
  if (serverConfigs.size() == 0) {
    return NULL;
  }
  std::vector<ServerConfig *> candidates;
  for (size_t i = 0; i < serverConfigs.size(); i++) {
    if (serverConfigs[i]->getServerName() == host) {
      candidates.push_back(serverConfigs[i]);
    }
  }
  ServerConfig *config = serverConfigs[0];
  if (candidates.size() > 0) {
    config = candidates[0];
  }
  return config;
}

static std::vector<std::string> strSplit(const std::string &str, char delim) {
  std::vector<std::string> result;
  size_t prev = 0;
  for (size_t i = 0; i < str.size(); i++) {
    if (str[i] == delim) {
      result.push_back(str.substr(prev, i - prev));
      prev = i + 1;
    }
  }
  if (prev < str.size()) {
    result.push_back(str.substr(prev, str.size() - prev));
  }
  return result;
}

static std::string strTrim(const std::string &str) {
  size_t start = 0;
  size_t end = str.size();
  for (size_t i = 0; i < str.size(); i++) {
    if (std::isspace(str[i])) {
      start++;
    } else {
      break;
    }
  }
  for (size_t i = str.size() - 1; i >= 0; i--) {
    if (std::isspace(str[i])) {
      end--;
    } else {
      break;
    }
  }
  return str.substr(start, end - start);
}

static bool isParentPath(const std::string &parent, const std::string &child) {
  if (parent.size() > child.size()) {
    return false;
  }
  const std::vector<std::string> parentSplit = strSplit(parent, '/');
  const std::vector<std::string> childSplit = strSplit(child, '/');
  if (parentSplit.size() > childSplit.size()) {
    return false;
  }
  for (size_t i = 0; i < parentSplit.size(); i++) {
    if (parentSplit[i] != childSplit[i]) {
      return false;
    }
  }
  return true;
}

static const LocationConfig *selectLocationConfig(
    const std::vector<LocationConfig> &locations, const std::string &uri) {
  const LocationConfig *res = NULL;
  size_t maxLocationLen = 0;
  for (size_t i = 0; i < locations.size(); i++) {
    const std::string &locationURI = locations[i].getUri();
    if (isParentPath(locationURI, uri) == false) {
      continue;
    }
    if (maxLocationLen > locationURI.size()) {
      continue;
    }
    if (locationURI[locationURI.size() - 1] != '/' && locationURI != uri) {
      continue;
    }
    maxLocationLen = locationURI.size();
    res = &locations[i];
  }
  return res;
}

const LocationConfig *ClientEventController::getLocationConfig() {
  if (config_ == NULL) {
    ServerConfig *serverConfig =
        selectServerConfig(request_, getServerConfigs());
    if (serverConfig == NULL) {
      response_.setStatusCode(400);
      return NULL;
    }
    config_ = selectLocationConfig(serverConfig->getLocationConfigs(),
                                   strTrim(request_.getUri()));
    std::cout << "debug - selected location path: " << config_->getUri()
              << std::endl;
  }
  return config_;
}

enum EventController::returnType ClientEventController::handleEvent(
    const Multiplexer::Event &event) {
  if (event.filter == WEB_READ) {
    recvBuffer_.resize(MAX_BUFFER_SIZE);
    int size = recv(fd_, recvBuffer_.data(), MAX_BUFFER_SIZE, 0);
    if (size == -1) {
      std::cout << "debug: read error" << std::endl;
      clear(true);
      return FAIL;
    }
    if (size == 0) {
      std::cout << "debug: closed socket(" << fd_ << ")" << std::endl;
      clear(true);
      return SUCCESS;
    }
    recvBuffer_.resize(size);
  }
  if (event.filter == WEB_WRITE) {
    int size = stream_.writeToClient(fd_);
    if (size == -1) {
      clear(true);
      return FAIL;
    }
    if (stream_.isEOF()) {
      clear(false);
      return SUCCESS;
    }
  }
  if (event.filter == WEB_TIMEOUT) {
    std::cout << "debug: timeout - close client" << std::endl;
    clear(true);
    return SUCCESS;
  }
  if (processor_ == NULL) {
    std::cout << "error: no processor" << std::endl;
    return FAIL;
  }
  ProcessResult processResult = nextProcessor();
  while (processResult.nextProcessor_) {
    processResult = nextProcessor();
  }
  if (processResult.error_) {
    std::cout << "error: fatal close";
    clear(true);
    return FAIL;
  }
  return PENDING;
}

ProcessResult ClientEventController::nextProcessor() {
  ProcessResult res = processor_->process();
  if (res.error_) {
    return res;
  }
  if (res.readOn_) {
    Multiplexer::getInstance().addReadEvent(fd_, this);
  }
  if (res.readOff_) {
    Multiplexer::getInstance().removeReadEvent(fd_, this);
  }
  if (res.writeOn_) {
    Multiplexer::getInstance().addWriteEvent(fd_, this);
  }
  if (res.writeOff_) {
    Multiplexer::getInstance().removeWriteEvent(fd_, this);
  }
  if (res.spendReadBuffer_) {
    recvBuffer_.erase(recvBuffer_.begin(),
                      recvBuffer_.begin() + res.spendReadBuffer_);
  }
  if (res.nextProcessor_) {
    delete processor_;
    processor_ = res.nextProcessor_;
  }
  return res;
}

void ClientEventController::clear(bool forceClose) {
  Multiplexer::getInstance().removeReadEvent(fd_, this);
  Multiplexer::getInstance().removeWriteEvent(fd_, this);
  Multiplexer::getInstance().removeTimeoutEvent(fd_, this);
  if (response_.hasHeader("Connection") &&
      response_.getHeader("Connection") == "keep-alive" &&
      forceClose == false) {
    const std::vector<ServerConfig *> &configs = getServerConfigs();
    ClientEventController::addEventController(fd_, configs);
  } else {
    close(fd_);
  }
}

std::ostream &operator<<(std::ostream &o,
                         const std::map<std::string, std::string> &rhs) {
  for (std::map<std::string, std::string>::const_iterator it = rhs.begin();
       it != rhs.end(); it++) {
    o << it->first << ": " << it->second << '\n';
  }
  return o;
}
