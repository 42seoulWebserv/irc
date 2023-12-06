#include "Multiplexer.hpp"

#include <sys/event.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "EventController.hpp"
#include "fcntl.h"

Multiplexer::Multiplexer() {
  fd_ = kqueue();
  fcntl(fd_, F_SETFL, O_NONBLOCK);
  fcntl(fd_, F_SETFD, FD_CLOEXEC);
}

Multiplexer& Multiplexer::getInstance() {
  static Multiplexer instance;
  return instance;
}

void Multiplexer::addReadEvent(int fd, EventController* udata) {
  struct kevent event;
  EV_SET(&event, fd, EVFILT_READ, EV_ADD, 0, 0, udata);
  kevent(fd_, &event, 1, NULL, 0, 0);
}

void Multiplexer::addTimeoutEvent(int fd, EventController* udata) {
  struct kevent timer_event;
  EV_SET(&timer_event, fd, EVFILT_TIMER, EV_ADD | EV_ONESHOT, 0, TIMEOUT * 1000,
         udata);
  kevent(fd_, &timer_event, 1, NULL, 0, 0);
}

void Multiplexer::addWriteEvent(int fd, EventController* udata) {
  struct kevent event;
  EV_SET(&event, fd, EVFILT_WRITE, EV_ADD, 0, 0, udata);
  kevent(fd_, &event, 1, NULL, 0, 0);
}

void Multiplexer::removeReadEvent(int fd, EventController* udata) {
  struct kevent event;
  EV_SET(&event, fd, EVFILT_READ, EV_DELETE, 0, 0, udata);
  kevent(fd_, &event, 1, NULL, 0, 0);
}

void Multiplexer::removeWriteEvent(int fd, EventController* udata) {
  struct kevent event;
  EV_SET(&event, fd, EVFILT_WRITE, EV_DELETE, 0, 0, udata);
  kevent(fd_, &event, 1, NULL, 0, 0);
}

void Multiplexer::removeTimeoutEvent(int fd, EventController* udata) {
  struct kevent timer_event;
  EV_SET(&timer_event, fd, EVFILT_TIMER, EV_DELETE, 0, 0, udata);
  kevent(fd_, &timer_event, 1, NULL, 0, 0);
}

std::vector<Multiplexer::Event> Multiplexer::wait(int size) {
  std::vector<Event> result;
  struct kevent eventList[size];
  int number = kevent(fd_, 0, 0, eventList, size, NULL);
  for (int i = 0; i < number; i++) {
    Event event;
    event.controller = reinterpret_cast<EventController*>(eventList[i].udata);
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

void Multiplexer::addDeleteController(EventController* controller) {
  if (controller == NULL) {
    return;
  }
  controller->setDeprecated(true);
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
