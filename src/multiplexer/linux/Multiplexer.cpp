#include "Multiplexer.hpp"

#include <sys/epoll.h>

Multiplexer::Multiplexer() : fd_(epoll_create1(EPOLL_CLOEXEC)) {}

Multiplexer& Multiplexer::getInstance() {
  static Multiplexer instance;
  return instance;
}

void Multiplexer::addReadEvent(int fd, void* udata) {
  struct epoll_event event;
  event.data.ptr = udata;
  event.events = EPOLLIN;
  epoll_ctl(fd_, EPOLL_CTL_ADD, fd, &event);
}

void Multiplexer::addReadEventWithClearFlag(int fd, void* udata) {
  struct epoll_event event;
  event.data.ptr = udata;
  event.events = EPOLLIN | EPOLLET;
  epoll_ctl(fd_, EPOLL_CTL_ADD, fd, &event);
}

void Multiplexer::addWriteEvent(int fd, void* udata) {
  struct epoll_event event;
  event.data.ptr = udata;
  event.events = EPOLLOUT;
  epoll_ctl(fd_, EPOLL_CTL_ADD, fd, &event);
}

void Multiplexer::removeReadEvent(int fd, void* udata) {
  struct epoll_event event;
  event.data.ptr = udata;
  event.events = EPOLLIN;
  epoll_ctl(fd_, EPOLL_CTL_DEL, fd, &event);
}

void Multiplexer::removeWriteEvent(int fd, void* udata) {
  struct epoll_event event;
  event.data.ptr = udata;
  event.events = EPOLLOUT;
  epoll_ctl(fd_, EPOLL_CTL_DEL, fd, &event);
}

void Multiplexer::addTimeoutEvent(int fd, void* udata) {
  // nothing to do
  (void)fd;
  (void)udata;
}

void Multiplexer::removeTimeoutEvent(int fd, void* udata) {
  // nothing to do
  (void)fd;
  (void)udata;
}
std::vector<Multiplexer::Event> Multiplexer::wait(int size) {
  std::vector<Event> result;
  struct epoll_event eventList[size];
  int eventCount = epoll_wait(fd_, eventList, size, -1);
  for (int i = 0; i < eventCount; i++) {
    Event event;
    event.ident = eventList[i].data.fd;
    event.udata = eventList[i].data.ptr;
    event.filter = 0;
    if (eventList[i].events & EPOLLIN) {
      event.filter = WEB_READ;
    }
    if (eventList[i].events & EPOLLOUT) {
      event.filter = WEB_WRITE;
    }
    if (event.filter == 0) {
      continue;
    }
    result.push_back(event);
  }
  return result;
}
