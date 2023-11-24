#include "EpollMultiplexer.hpp"

#include <sys/epoll.h>

EpollMultiplexer::EpollMultiplexer() : ep_(epoll_create1(EPOLL_CLOEXEC)) {}

EpollMultiplexer& EpollMultiplexer::getInstance() {
  static EpollMultiplexer instance;
  return instance;
}

void EpollMultiplexer::addReadEvent(int fd, void* udata) {
  struct epoll_event event;
  event.data.ptr = udata;
  event.events = EPOLLIN;
  epoll_ctl(ep_, EPOLL_CTL_ADD, fd, &event);
}

void EpollMultiplexer::addReadEventWithClearFlag(int fd, void* udata) {
  struct epoll_event event;
  event.data.ptr = udata;
  event.events = EPOLLIN | EPOLLET;
  epoll_ctl(ep_, EPOLL_CTL_ADD, fd, &event);
}

void EpollMultiplexer::addWriteEvent(int fd, void* udata) {
  struct epoll_event event;
  event.data.ptr = udata;
  event.events = EPOLLOUT;
  epoll_ctl(ep_, EPOLL_CTL_ADD, fd, &event);
}

void EpollMultiplexer::removeReadEvent(int fd, void* udata) {
  struct epoll_event event;
  event.data.ptr = udata;
  event.events = EPOLLIN;
  epoll_ctl(ep_, EPOLL_CTL_DEL, fd, &event);
}

void EpollMultiplexer::removeWriteEvent(int fd, void* udata) {
  struct epoll_event event;
  event.data.ptr = udata;
  event.events = EPOLLOUT;
  epoll_ctl(ep_, EPOLL_CTL_DEL, fd, &event);
}

void EpollMultiplexer::addTimeoutEvent(int fd, void* udata) {
  // nothing to do
  (void)fd;
  (void)udata;
}

void EpollMultiplexer::removeTimeoutEvent(int fd, void* udata) {
  // nothing to do
  (void)fd;
  (void)udata;
}

int EpollMultiplexer::getEp() { return ep_; }
