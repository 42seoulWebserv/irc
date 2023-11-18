#include <sys/event.h>
#include <unistd.h>

#include <iostream>
#include <sstream>

#include "ClientEventController.hpp"
#include "EventController.hpp"

enum EventController::returnType ClientEventController::clientWrite(
    const struct kevent &event) {
  if (stream_.isEOF() == false) {
    stream_.writeToClient(clientSocket_);
    if (stream_.isEOF()) {
      return PENDING;
    }
  }
  if (response_.hasHeader("Connection") &&
      response_.getHeader("Connection") == "keep-alive") {
    ClientEventController::addEventController(kq_, clientSocket_,
                                              getServerConfigs());
    return SUCCESS;
  }
  close(clientSocket_);
  return SUCCESS;
}
