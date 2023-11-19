#include "FileReadEventController.hpp"

#include <fcntl.h>
#include <sys/event.h>  // kevent
#include <unistd.h>

#include <exception>

FileReadEventController::FileReadEventController(const std::string &filepath,
                                                 IObserver<Event> *observer)
    : filepath_(filepath), observer_(observer) {
  fd_ = open(filepath_.c_str(), O_RDONLY);
  if (fd_ == -1) {
    throw std::invalid_argument("file open error");
  }
  fcntl(fd_, F_SETFL, O_NONBLOCK);
  fcntl(fd_, F_SETFD, FD_CLOEXEC);
  KqueueMultiplexer::getInstance().addReadEvent(fd_, this);
}

FileReadEventController *FileReadEventController::addEventController(
    const std::string &filepath, IObserver<Event> *observer) {
  try {
    return new FileReadEventController(filepath, observer);
  } catch (...) {
    if (observer) {
      observer->onEvent(Event(FileReadEventController::FAIL, ""));
      return NULL;
    }
  }
}

EventController::returnType FileReadEventController::handleEvent(
    const struct kevent &event) {
  content_.reserve(event.data);
  char buffer[FILE_READ_BUFF_SIZE];
  intptr_t i = 0;
  while (i < event.data) {
    int size = read(fd_, buffer, FILE_READ_BUFF_SIZE);
    if (size == -1) {
      close(fd_);
      if (observer_) {
        observer_->onEvent(Event(FileReadEventController::FAIL, content_));
      }
      return EventController::FAIL;
    }
    content_.append(buffer, size);
    i += size;
  }
  close(fd_);
  if (observer_) {
    observer_->onEvent(Event(FileReadEventController::SUCCESS, content_));
  }
  return EventController::SUCCESS;
}

FileReadEventController::Event::Event(EventType type,
                                      const std::string &content)
    : type_(type), content_(content) {}
