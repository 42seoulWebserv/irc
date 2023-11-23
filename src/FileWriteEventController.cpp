#include "FileWriteEventController.hpp"

#include <fcntl.h>
#include <sys/event.h>  // kevent
#include <unistd.h>

#include <exception>

#include "KqueueMultiplexer.hpp"

FileWriteEventController::FileWriteEventController(const std::string &filepath,
                                                   const std::string &content,
                                                   IObserver<Event> *observer)
    : filepath_(filepath),
      content_(content),
      offset_(0),
      observer_(observer),
      isCanceled_(false) {
  file_ = fopen(filepath.c_str(), "aw");
  if (file_ == NULL) {
    throw std::invalid_argument("file open error");
  }
  fd_ = fileno(file_);
  fcntl(fd_, F_SETFL, O_NONBLOCK);
  fcntl(fd_, F_SETFD, FD_CLOEXEC);
  KqueueMultiplexer::getInstance().addWriteEvent(fd_, this);
}

FileWriteEventController *FileWriteEventController::addEventController(
    const std::string &filepath, const std::string &content,
    IObserver<Event> *observer) {
  try {
    return new FileWriteEventController(filepath, content, observer);
  } catch (...) {
    if (observer) {
      observer->onEvent(Event(FileWriteEventController::FAIL));
    }
    return NULL;
  }
}

EventController::returnType FileWriteEventController::handleEvent(
    const struct kevent &event) {
  if (isCanceled_) {
    fclose(file_);
    return EventController::FAIL;
  }
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

void FileWriteEventController::cancel() { isCanceled_ = true; }
