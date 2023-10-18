#include <iostream>
#include <unistd.h>
#include <sys/event.h>

#include "ClientEventController.hpp"
#include "EventController.hpp"

ClientEventController::ClientEventController(int kq, int clientSocket) : kq_(kq), clientSocket_(clientSocket) {}

ClientEventController::ClientEventController(const ClientEventController &src) : kq_(src.kq_), clientSocket_(src.clientSocket_) {}

ClientEventController &ClientEventController::operator=(const ClientEventController &rhs) {
  this->kq_ = rhs.kq_;
  this->clientSocket_ = rhs.clientSocket_;
  return *this;
}

ClientEventController::~ClientEventController() {}

enum EventController::returnType ClientEventController::handleEvent(const struct kevent &event) {
  if (event.filter == EVFILT_READ) {
    return clientRead(event);
  }
  else if (event.filter == EVFILT_WRITE) { // cgi
    return clientWrite(event);
  }
  else if (event.filter == EVFILT_TIMER) {
    return clientTimeout(event);
  }
  else {
    return FAIL;
  }
  return SUCCESS;
}