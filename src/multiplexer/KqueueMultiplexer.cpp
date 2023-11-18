#include "KqueueMultiplexer.hpp"

#include <unistd.h>
KqueueMultiplexer::KqueueMultiplexer() { kq_ = kqueue(); }

void KqueueMultiplexer::addReadEvent(int fd, void* udata) {
  struct kevent event;
  EV_SET(&event, fd, EVFILT_READ, EV_ADD, 0, 0, udata);
  kevent(kq_, &event, 1, NULL, 0, 0);
}

void KqueueMultiplexer::addReadEventWithClearFlag(int fd, void* udata) {
  struct kevent event;
  EV_SET(&event, fd, EVFILT_READ, EV_ADD | EV_CLEAR, 0, 0, udata);
  kevent(kq_, &event, 1, NULL, 0, 0);
}

void KqueueMultiplexer::addWriteEvent(int fd, void* udata) {
  struct kevent event;
  EV_SET(&event, fd, EVFILT_WRITE, EV_ADD, 0, 0, udata);
  kevent(kq_, &event, 1, NULL, 0, 0);
}

void KqueueMultiplexer::removeReadEvent(int fd, void* udata) {
  struct kevent event;
  EV_SET(&event, fd, EVFILT_READ, EV_DELETE, 0, 0, udata);
  kevent(kq_, &event, 1, NULL, 0, 0);
}

void KqueueMultiplexer::removeWriteEvent(int fd, void* udata) {
  struct kevent event;
  EV_SET(&event, fd, EVFILT_WRITE, EV_DELETE, 0, 0, udata);
  kevent(kq_, &event, 1, NULL, 0, 0);
}

int KqueueMultiplexer::getKq() { return kq_; }
