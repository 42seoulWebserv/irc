#include "FileReadEventController.hpp"

#include <fcntl.h>
#include <sys/event.h>  // kevent

#include <exception>

FileReadEventController::FileReadEventController(const std::string &filepath,
                                                 IObserver<Event> *observer,
                                                 DataStream *stream)
    : filepath_(filepath), observer_(observer), dataStream_(stream) {
  file_ = fopen(filepath_.c_str(), "r");
  if (file_ == NULL) {
    throw std::invalid_argument("file open error");
  }
  fd_ = fileno(file_);
  fcntl(fd_, F_SETFL, O_NONBLOCK);
  fcntl(fd_, F_SETFD, FD_CLOEXEC);
  KqueueMultiplexer::getInstance().addReadEvent(fd_, this);
}

FileReadEventController *FileReadEventController::addEventController(
    const std::string &filepath, IObserver<Event> *observer,
    DataStream *stream) {
  try {
    return new FileReadEventController(filepath, observer, stream);
  } catch (...) {
    if (observer) {
      observer->onEvent(Event(FileReadEventController::FAIL));
      return NULL;
    }
  }
}

EventController::returnType FileReadEventController::handleEvent(
    const struct kevent &event) {
  int size = dataStream_->readFile(fd_);
  if (size == DELAYED_FILE_READ) {
    return PENDING;
  }
  if (size == -1) {
    fclose(file_);
    if (observer_) {
      observer_->onEvent(Event(FileReadEventController::FAIL));
    }
    return EventController::FAIL;
  }
  if (size == 0) {
    fclose(file_);
    if (observer_) {
      observer_->onEvent(Event(FileReadEventController::SUCCESS));
    }
    return EventController::SUCCESS;
  }
  if (observer_) {
    observer_->onEvent(Event(FileReadEventController::SUCCESS));
  }
  return EventController::PENDING;
}

FileReadEventController::Event::Event(EventType type) : type_(type) {}
