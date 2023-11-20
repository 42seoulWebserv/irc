#include "FileWriteEventController.hpp"

#include <fcntl.h>
#include <sys/event.h>  // kevent
#include <unistd.h>

#include <exception>

#include "KqueueMultiplexer.hpp"

FileWriteEventController::FileWriteEventController(const std::string &filepath,
                                                   const std::string &content,
                                                   IObserver<Event> *observer)
    : filepath_(filepath), content_(content), offset_(0), observer_(observer) {
  fd_ = open(filepath_.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
  if (fd_ == -1) {
    throw std::invalid_argument("file open error");
  }
  fcntl(fd_, F_SETFL, O_NONBLOCK);
  fcntl(fd_, F_SETFD, FD_CLOEXEC);
  KqueueMultiplexer::getInstance().addReadEvent(fd_, this);
}

void FileWriteEventController::addEventController(const std::string &filepath,
                                                  const std::string &content,
                                                  IObserver<Event> *observer) {
  try {
    new FileWriteEventController(filepath, content, observer);
  } catch (...) {
    if (observer) {
      observer->onEvent(Event(FileWriteEventController::FAIL));
    }
  }
}

EventController::returnType FileWriteEventController::handleEvent(
    const struct kevent &event) {
  if (event.filter != EVFILT_WRITE) {
    std::cout << "unexpected event" << std::endl;
    close(fd_);
    if (observer_) {
      observer_->onEvent(Event(FileWriteEventController::FAIL));
    }
    return EventController::FAIL;
  }
  size_t size =
      write(fd_, content_.c_str() + offset_, content_.size() - offset_);
  offset_ += size;
  if (offset_ < content_.size()) {
    return PENDING;
  }
  close(fd_);
  if (observer_) {
    observer_->onEvent(Event(FileWriteEventController::SUCCESS));
  }
  return EventController::SUCCESS;
}

FileWriteEventController::Event::Event(EventType type) : type_(type) {}
