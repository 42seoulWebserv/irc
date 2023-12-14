#include "ClientEventController.hpp"

#include <fcntl.h>
#include <sys/socket.h>
#include <unistd.h>

#include "EventController.hpp"
#include "Log.hpp"
#include "StartProcessor.hpp"
#include "String.hpp"

ClientEventController::ClientEventController(int clientSocket)
    : EventController(new StartProcessor(*this)), config_(NULL) {
  fd_ = clientSocket;
  fcntl(fd_, F_SETFL, O_NONBLOCK);
  fcntl(fd_, F_SETFD, FD_CLOEXEC);
}

ClientEventController::~ClientEventController() {}

ClientEventController *ClientEventController::addEventController(
    int socket, const std::vector<ServerConfig *> &configs) {
  ClientEventController *client = new ClientEventController(socket);
  if (client == NULL) {
    return NULL;
  }
  client->serverConfigs_ = configs;
  Multiplexer::getInstance().addReadEvent(socket, client);
  Multiplexer::getInstance().addTimeoutEvent(socket, client);
  return client;
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

void ClientEventController::setBody(const std::string &body) { body_ = body; }

std::string &ClientEventController::getBody() { return body_; }

DataStream &ClientEventController::getResponseStream() {
  return responseStream_;
}

StringBuffer &ClientEventController::getRecvBuffer() { return recvBuffer_; }

FilePath ClientEventController::getRequestResourcePath() {
  const LocationConfig *config = getLocationConfig();
  if (config == NULL) {
    return FilePath();
  }
  FilePath path = config->getRootPath();
  path.append(request_.getUri());
  return path;
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

static bool isParentPath(const std::string &parent, const std::string &child) {
  if (parent.size() > child.size()) {
    return false;
  }
  const std::vector<std::string> parentSplit = String(parent).split("/");
  const std::vector<std::string> childSplit = String(child).split("/");
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
    ServerConfig *serverConfig = selectServerConfig(request_, serverConfigs_);
    if (serverConfig == NULL) {
      if (response_.getStatusCode() == 100) {
        response_.setStatusCode(400);
      }
      return NULL;
    }
    config_ = selectLocationConfig(serverConfig->getLocationConfigs(),
                                   String(request_.getUri()).trim());
    if (config_ == NULL) {
      if (response_.getStatusCode() == 100) {
        response_.setStatusCode(400);
      }
      return NULL;
    }
    Log::debug << fd_ << ":  selected location path: " << config_->getUri()
               << NL;
  }
  return config_;
}

void ClientEventController::print(Log &logger, const std::string &msg) {
  logger << fd_ << ": " << msg << NL;
}

void ClientEventController::init() {
  if (loopProcess()) {
    throw std::logic_error("error: init client event controller");
  }
}

void ClientEventController::handleEvent(const Multiplexer::Event &event) {
  if (event.filter == WEB_READ) {
    std::vector<char> buffer;
    buffer.resize(MAX_BUFFER_SIZE);
    int size = recv(fd_, buffer.data(), MAX_BUFFER_SIZE, 0);
    if (size == -1) {
      print(Log::info, "read error");
      clearForce();
      return;
    }
    if (size == 0) {
      clearForce();
      return;
    }
    buffer.resize(size);
    recvBuffer_.addBuffer(buffer);
  }
  if (event.filter == WEB_WRITE) {
    int size = responseStream_.popToClient(fd_);
    if (size == -1) {
      clearForce();
      return;
    }
    if (responseStream_.isEOF()) {
      clearKeepAlive();
      return;
    }
  }
  if (event.filter == WEB_TIMEOUT) {
    print(Log::info, "timeout");
    clearForce();
    return;
  }
  if (loopProcess()) {
    clearForce();
    return;
  }
}

void ClientEventController::clearKeepAlive() {
  if (response_.hasHeader("Connection") == false ||
      response_.getHeader("Connection") != "keep-alive") {
    clearForce();
    return;
  }
  Multiplexer::getInstance().addDeleteController(this);
  ClientEventController *client =
      ClientEventController::addEventController(fd_, serverConfigs_);
  if (client == NULL) {
    clearForce();
    return;
  }
  client->getRecvBuffer().addBuffer(getRecvBuffer().getBuffer());
}

void ClientEventController::clearForce() {
  Multiplexer::getInstance().addDeleteController(this);
  close(fd_);
  print(Log::info, "close socket");
}

std::ostream &operator<<(std::ostream &o,
                         const std::map<std::string, std::string> &rhs) {
  for (std::map<std::string, std::string>::const_iterator it = rhs.begin();
       it != rhs.end(); it++) {
    o << it->first << ": " << it->second << '\n';
  }
  return o;
}
