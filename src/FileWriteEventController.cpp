#include "FileWriteEventController.hpp"

#include <fcntl.h>
#include <sys/event.h>  // kevent
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <exception>

FileWriteEventController::FileWriteEventController(int kq,
                                                   const std::string &filepath,
                                                   const std::string &content,
                                                   IObserver<Event> *observer)
    : kq_(kq),
      filepath_(filepath),
      content_(content),
      offset_(0),
      observer_(observer) {
  const char *file = filepath.c_str();
  struct stat statbuf;
  if (stat(file, &statbuf) == 0 && access(file, W_OK)) {
    throw FileWriteException(FileWriteEventController::NOT_ACCESS);
  }
  fd_ = open(filepath_.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
  if (fd_ == -1) {
    throw FileWriteException(FileWriteEventController::FAIL);
  }
  fcntl(fd_, F_SETFL, O_NONBLOCK);
  fcntl(fd_, F_SETFD, FD_CLOEXEC);
  struct kevent event;
  EV_SET(&event, fd_, EVFILT_WRITE, EV_ADD, 0, 0, this);
  kevent(kq_, &event, 1, NULL, 0, 0);
}

void FileWriteEventController::addEventController(int kq,
                                                  const std::string &filepath,
                                                  const std::string &content,
                                                  IObserver<Event> *observer) {
  try {
    new FileWriteEventController(kq, filepath, content, observer);
  } catch (const FileWriteException &e) {
    if (observer == NULL) {
      return;
    }
    observer->onEvent(Event(e.type_));
  } catch (...) {
    observer->onEvent(Event(FileWriteEventController::FAIL));
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

FileWriteEventController::FileWriteException::FileWriteException(EventType type)
    : type_(type) {}