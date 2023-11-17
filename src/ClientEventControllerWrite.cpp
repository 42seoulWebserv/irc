#include <sys/event.h>
#include <unistd.h>

#include <iostream>
#include <sstream>

#include "ClientEventController.hpp"
#include "EventController.hpp"

enum EventController::returnType ClientEventController::clientWrite(
    const struct kevent &event) {
  if (response_.hasHeader("Connection") &&
      response_.getHeader("Connection") == "keep-alive") {
    ClientEventController::addEventController(kq_, clientSocket_,
                                              getServerConfigs());
    evSet(EVFILT_WRITE, EV_DELETE);
    return SUCCESS;
  }
  return SUCCESS;
}
