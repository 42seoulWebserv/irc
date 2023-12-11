#include "Multiplexer.hpp"

#include <sys/epoll.h>
#include <cstddef>

#include "EventController.hpp"

Multiplexer::Multiplexer() : fd_(epoll_create1(EPOLL_CLOEXEC)) {}

Multiplexer& Multiplexer::getInstance() {
  static Multiplexer instance;
  return instance;
}

void Multiplexer::addReadEvent(int fd, EventController* udata) {
  struct epoll_event event;
  event.data.ptr = udata;
  event.events = EPOLLIN;
  epoll_ctl(fd_, EPOLL_CTL_ADD, fd, &event);
}

void Multiplexer::addWriteEvent(int fd, EventController* udata) {
  struct epoll_event event;
  event.data.ptr = udata;
  event.events = EPOLLOUT;
  epoll_ctl(fd_, EPOLL_CTL_ADD, fd, &event);
}

void Multiplexer::removeReadEvent(int fd, EventController* udata) {
  struct epoll_event event;
  event.data.ptr = udata;
  event.events = EPOLLIN;
  epoll_ctl(fd_, EPOLL_CTL_DEL, fd, &event);
}

void Multiplexer::removeWriteEvent(int fd, EventController* udata) {
  struct epoll_event event;
  event.data.ptr = udata;
  event.events = EPOLLOUT;
  epoll_ctl(fd_, EPOLL_CTL_DEL, fd, &event);
}

void Multiplexer::addTimeoutEvent(int fd, EventController* udata) {
  // nothing to do
  (void)fd;
  (void)udata;
}

void Multiplexer::removeTimeoutEvent(int fd, EventController* udata) {
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
    event.controller =
        reinterpret_cast<EventController*>(eventList[i].data.ptr);
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

void Multiplexer::addDeleteController(EventController* controller) {
  if (controller == NULL) {
    return;
  }
  removeReadEvent(controller->getFd(), controller);
  removeWriteEvent(controller->getFd(), controller);
  removeTimeoutEvent(controller->getFd(), controller);
  deleteList_.insert(controller);
}

void Multiplexer::deleteAddedControllers() {
  if (deleteList_.size() == 0) {
    return;
  }
  std::set<EventController*> deleteList = deleteList_;
  deleteList_.clear();
  std::set<EventController*>::const_iterator it;
  for (it = deleteList.begin(); it != deleteList.end(); it++) {
    delete *it;
  }
  deleteAddedControllers();
}
