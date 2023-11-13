#include <sys/event.h>
#include <unistd.h>

#include <iostream>
#include <sstream>

#include "ClientEventController.hpp"
#include "EventController.hpp"

enum EventController::returnType ClientEventController::clientWrite(
    const struct kevent &event) {
  if (response_.hasHeader("Host") == false) {
    response_.setHeader("Host", request_.getHeader("Host"));
  }
  if (response_.getVersion().empty()) {
    response_.setVersion(request_.getVersion());
  }
  const std::string str = response_.toString();
  write(event.ident, str.c_str(), str.size());
  evSet(EVFILT_WRITE, EV_DELETE);
  return PENDING;
}
