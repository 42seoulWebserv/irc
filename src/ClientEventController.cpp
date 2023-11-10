#include "ClientEventController.hpp"

#include <sys/event.h>
#include <unistd.h>

#include <iostream>

#include "EventController.hpp"

ClientEventController::ClientEventController(int kq, int clientSocket)
    : readStatus_(START_LINE),
      kq_(kq),
      statusCode_(0),
      clientSocket_(clientSocket),
      processor_(NULL) {}

ClientEventController::ClientEventController(const ClientEventController &src)
    : readStatus_(src.readStatus_),
      kq_(src.kq_),
      statusCode_(src.statusCode_),
      clientSocket_(src.clientSocket_),
      processor_(NULL) {}

ClientEventController &ClientEventController::operator=(
    const ClientEventController &rhs) {
  this->kq_ = rhs.kq_;
  this->statusCode_ = rhs.statusCode_;
  this->readStatus_ = rhs.readStatus_;
  this->clientSocket_ = rhs.clientSocket_;
  return *this;
}

ClientEventController::~ClientEventController() {
  if (processor_ != NULL) {
    delete processor_;
  }
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

void ClientEventController::evSet(int filter, int action) {
  struct kevent clientEvent;
  EV_SET(&clientEvent, this->clientSocket_, filter, action, 0, 0, this);
  kevent(kq_, &clientEvent, 1, NULL, 0, 0);
}

void ClientEventController::onEvent(const ResponseVO &p) {
  // TODO: 버전 체크, 필요하다면 헤더 추가 등의 작업 가능
  response_ = p;
  evSet(EVFILT_WRITE, EV_ADD);
}
