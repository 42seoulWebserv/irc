#include <iostream>
#include <unistd.h>
#include <sys/event.h>

#include "ClientEventController.hpp"
#include "EventController.hpp"

enum EventController::returnType ClientEventController::clientWrite(const struct kevent &event)
{
  write(event.ident, "HTTP/1.1 200 OK\r\n", 17);
  write(event.ident, "Host: localhost:420\r\n", 21);
  write(event.ident, "Content-Length: 6\r\n", 19);
  write(event.ident, "Content-Type: text/html\r\n", 25);
  write(event.ident, "\r\n", 2);
  write(event.ident, "hello\n", 6);
  evSet(EVFILT_WRITE, EV_DELETE);
  return SUCCESS;
}
