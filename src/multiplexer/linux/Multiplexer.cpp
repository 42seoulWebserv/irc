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
