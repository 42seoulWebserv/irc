#include "ClientEventController.hpp"

#include <sys/event.h>
#include <unistd.h>

#include <iostream>

#include "EventController.hpp"
#include "ParseRequestProcessor.hpp"

// constructor / destructor
ClientEventController::ClientEventController(int clientSocket)
    : clientSocket_(clientSocket), config_(NULL), processor_(NULL) {
  processor_ = new ParseRequestProcessor(*this);
  while (nextProcessor()) {
    // nothing
  }
}

ClientEventController::~ClientEventController() { delete processor_; }

void ClientEventController::addEventController(
    int socket, const std::vector<ServerConfig *> &configs) {
  ClientEventController *client = new ClientEventController(socket);
  client->setServerConfigs(configs);
  KqueueMultiplexer::getInstance().addReadEventWithClearFlag(socket, client);
  KqueueMultiplexer::getInstance().addTimeoutEvent(socket, client);
}

// IClient
const Request &ClientEventController::getRequest() const { return request_; }

const Response &ClientEventController::getResponse() const { return response_; }

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
    const struct kevent &event) {
  if (event.filter == EVFILT_READ && event.flags & EV_EOF) {
    std::cout << "debug: closed socket(" << clientSocket_ << ")" << std::endl;
    clear(true);
    return SUCCESS;
  }
  if (event.filter == EVFILT_READ) {
    recvBuffer_.resize(event.data);
    int size = recv(clientSocket_, recvBuffer_.data(), event.data, 0);
    if (size == -1) {
      std::cout << "debug: read error" << std::endl;
      clear(true);
      return FAIL;
    }
    if (size == 0) {
      std::cout << "debug: closed socket(" << clientSocket_ << ")" << std::endl;
      clear(true);
      return SUCCESS;
    }
    recvBuffer_.resize(size);
  }
  if (event.filter == EVFILT_WRITE) {
    stream_.writeToClient(clientSocket_);
    if (stream_.isEOF()) {
      clear(false);
      return SUCCESS;
    }
  }
  if (event.filter == EVFILT_TIMER) {
    std::cout << "debug: timeout - close client" << std::endl;
    clear(true);
    return SUCCESS;
  }
  if (processor_ == NULL) {
    std::cout << "error: no processor" << std::endl;
    return FAIL;
  }
  while (nextProcessor()) {
    // nothing
  }
  return PENDING;
}

bool ClientEventController::nextProcessor() {
  ProcessResult res = processor_->process();
  if (res.readOn_) {
    KqueueMultiplexer::getInstance().addReadEvent(clientSocket_, this);
  }
  if (res.readOff_) {
    KqueueMultiplexer::getInstance().removeReadEvent(clientSocket_, this);
  }
  if (res.writeOn_) {
    KqueueMultiplexer::getInstance().addWriteEvent(clientSocket_, this);
  }
  if (res.writeOff_) {
    KqueueMultiplexer::getInstance().removeWriteEvent(clientSocket_, this);
  }
  if (res.request) {
    request_ = *res.request;
  }
  if (res.response) {
    response_ = *res.response;
  }
  if (res.error_) {
    clear(false);
    return FAIL;
  }
  if (res.status_ != 0) {
    response_.setStatusCode(res.status_);
  }
  if (res.spendReadBuffer_) {
    recvBuffer_.erase(recvBuffer_.begin(),
                      recvBuffer_.begin() + res.spendReadBuffer_);
  }
  if (res.nextProcessor_) {
    delete processor_;
    processor_ = res.nextProcessor_;
  }
  return res.nextProcessor_ != NULL;
}

void ClientEventController::clear(bool forceClose) {
  KqueueMultiplexer::getInstance().removeReadEvent(clientSocket_, this);
  KqueueMultiplexer::getInstance().removeWriteEvent(clientSocket_, this);
  KqueueMultiplexer::getInstance().removeTimeoutEvent(clientSocket_, this);
  if (response_.hasHeader("Connection") &&
      response_.getHeader("Connection") == "keep-alive" &&
      forceClose == false) {
    const std::vector<ServerConfig *> &configs = getServerConfigs();
    ClientEventController::addEventController(clientSocket_, configs);
  } else {
    close(clientSocket_);
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
