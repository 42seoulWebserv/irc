#include <sys/event.h>
#include <unistd.h>

#include <iostream>
#include <sstream>

#include "ClientEventController.hpp"
#include "EventController.hpp"

enum EventController::returnType ClientEventController::clientWrite(
    const struct kevent &event) {
  write(event.ident, "HTTP/1.1 200 OK\r\n", 17);
  write(event.ident, "Host: localhost:420\r\n", 21);
  write(event.ident, "Content-Length: ", 16);
  std::stringstream ss;
  ss << response_.getBody().size();
  write(event.ident, ss.str().c_str(), ss.str().size());
  write(event.ident, "\r\n", 2);
  write(event.ident, "Content-Type: text/html\r\n", 25);
  write(event.ident, "\r\n", 2);
  write(event.ident, response_.getBody().c_str(), response_.getBody().size());
  evSet(EVFILT_WRITE, EV_DELETE);  // write 이벤트를 안 받는다
  return PENDING;
}
