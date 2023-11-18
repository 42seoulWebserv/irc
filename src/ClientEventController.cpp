#include "ClientEventController.hpp"

#include <sys/event.h>
#include <unistd.h>

#include <iostream>

#include "EventController.hpp"

ClientEventController::ClientEventController(int clientSocket)
    : readStatus_(START_LINE),
      statusCode_(0),
      clientSocket_(clientSocket),
      config_(NULL),
      processor_(NULL) {}

ClientEventController::ClientEventController(const ClientEventController &src)
    : readStatus_(src.readStatus_),
      statusCode_(src.statusCode_),
      clientSocket_(src.clientSocket_),
      config_(NULL),
      processor_(NULL) {}

ClientEventController &ClientEventController::operator=(
    const ClientEventController &rhs) {
  this->statusCode_ = rhs.statusCode_;
  this->readStatus_ = rhs.readStatus_;
  this->clientSocket_ = rhs.clientSocket_;
  this->config_ = rhs.config_;
  return *this;
}

ClientEventController::~ClientEventController() {
  if (processor_ != NULL) {
    delete processor_;
  }
}

void ClientEventController::addEventController(
    int socket, const std::vector<ServerConfig *> &configs) {
  struct timespec timeout = {10, 0};  // 10 seconds

  ClientEventController *client = new ClientEventController(socket);
  client->setServerConfigs(configs);
  KqueueMultiplexer::getInstance().addReadEventWithClearFlag(socket, client);
}

enum EventController::returnType ClientEventController::handleEvent(
    const struct kevent &event) {
  if (event.filter == EVFILT_READ) {
    return clientRead(event);
  } else if (event.filter == EVFILT_WRITE) {  // cgi
    return clientWrite(event);
  } else if (event.filter == EVFILT_TIMER) {
    return clientTimeout(event);
  } else {
    return FAIL;
  }
  return SUCCESS;
}

std::ostream &operator<<(std::ostream &o,
                         const std::map<std::string, std::string> &rhs) {
  for (std::map<std::string, std::string>::const_iterator it = rhs.begin();
       it != rhs.end(); it++) {
    o << it->first << ": " << it->second << '\n';
  }
  return o;
}

void ClientEventController::onEvent(const Response &p) {
  // TODO: 버전 체크, 필요하다면 헤더 추가 등의 작업 가능
  response_ = p;
  KqueueMultiplexer::getInstance().addWriteEvent(clientSocket_, this);
}
