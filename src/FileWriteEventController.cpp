#include "FileWriteEventController.hpp"

#include <fcntl.h>
#include <sys/event.h>  // kevent
#include <unistd.h>

#include <exception>

FileWriteEventController::FileWriteEventController(int kq,
                                                   const std::string &filepath,
                                                   const std::string &content,
                                                   IFileObserver *observer)
    : kq_(kq),
      filepath_(filepath),
      content_(content),
      offset_(0),
      observer_(observer) {
  fd_ = open(filepath_.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
  if (fd_ == -1) {
    throw std::invalid_argument("file open error");
  }
  fcntl(fd_, F_SETFL, O_NONBLOCK);
  fcntl(fd_, F_SETFD, FD_CLOEXEC);
  struct kevent event;
  EV_SET(&event, fd_, EVFILT_WRITE, EV_ADD, 0, 0, this);
  kevent(kq_, &event, 1, NULL, 0, 0);
}

EventController::returnType FileWriteEventController::handleEvent(
    const struct kevent &event) {
  if (event.filter != EVFILT_WRITE) {
    std::cout << "unexpected event" << std::endl;
    close(fd_);
    return FAIL;
  }
  size_t size =
      write(fd_, content_.c_str() + offset_, content_.size() - offset_);
  offset_ += size;
  if (offset_ < content_.size()) {
    return PENDING;
  }
  close(fd_);
  if (observer_) {
    observer_->fileEvent(FILE_WRITE | FILE_SUCCESS, "success");
  }
  return SUCCESS;
}
