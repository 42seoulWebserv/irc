#include "Multiplexer.hpp"

#include <sys/event.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

Multiplexer::Multiplexer() { fd_ = kqueue(); }

Multiplexer& Multiplexer::getInstance() {
  static Multiplexer instance;
  return instance;
}

void Multiplexer::addReadEvent(int fd, void* udata) {
  struct kevent event;
  EV_SET(&event, fd, EVFILT_READ, EV_ADD, 0, 0, udata);
  kevent(fd_, &event, 1, NULL, 0, 0);
}

void Multiplexer::addReadEventWithClearFlag(int fd, void* udata) {
  struct kevent event;
  EV_SET(&event, fd, EVFILT_READ, EV_ADD | EV_CLEAR, 0, 0, udata);
  kevent(fd_, &event, 1, NULL, 0, 0);
}

void Multiplexer::addTimeoutEvent(int fd, void* udata) {
  struct kevent timer_event;
  EV_SET(&timer_event, fd, EVFILT_TIMER, EV_ADD | EV_ONESHOT, 0, TIMEOUT * 1000,
         udata);
  kevent(fd_, &timer_event, 1, NULL, 0, 0);
}

void Multiplexer::addWriteEvent(int fd, void* udata) {
  struct kevent event;
  EV_SET(&event, fd, EVFILT_WRITE, EV_ADD, 0, 0, udata);
  kevent(fd_, &event, 1, NULL, 0, 0);
}

void Multiplexer::removeReadEvent(int fd, void* udata) {
  struct kevent event;
  EV_SET(&event, fd, EVFILT_READ, EV_DELETE, 0, 0, udata);
  kevent(fd_, &event, 1, NULL, 0, 0);
}

void Multiplexer::removeWriteEvent(int fd, void* udata) {
  struct kevent event;
  EV_SET(&event, fd, EVFILT_WRITE, EV_DELETE, 0, 0, udata);
  kevent(fd_, &event, 1, NULL, 0, 0);
}

void Multiplexer::removeTimeoutEvent(int fd, void* udata) {
  struct kevent timer_event;
  EV_SET(&timer_event, fd, EVFILT_TIMER, EV_DELETE, 0, 0, udata);
  kevent(fd_, &timer_event, 1, NULL, 0, 0);
}

std::vector<Multiplexer::Event> Multiplexer::wait(int size) {
  std::vector<Event> result;
  struct kevent eventList[size];
  int number = kevent(fd_, 0, 0, eventList, 5, NULL);
  for (int i = 0; i < number; i++) {
    Event event;
    event.ident = eventList[i].ident;
    event.udata = eventList[i].udata;
    if (eventList[i].filter == EVFILT_READ) {
      event.filter = WEB_READ;
    } else if (eventList[i].filter == EVFILT_WRITE) {
      event.filter = WEB_WRITE;
    } else if (eventList[i].filter == EVFILT_TIMER) {
      event.filter = WEB_TIMEOUT;
    } else {
      continue;
    }
    result.push_back(event);
  }
  return result;
}
