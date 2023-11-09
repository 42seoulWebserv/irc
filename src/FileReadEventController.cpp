#include "FileReadEventController.hpp"

#include <fcntl.h>
#include <sys/event.h>  // kevent
#include <unistd.h>

#include <exception>

FileReadEventController::FileReadEventController(int kq,
                                                 const std::string &filepath,
                                                 IFileObserver *observer)
    : kq_(kq), filepath_(filepath), observer_(observer) {
  fd_ = open(filepath_.c_str(), O_RDONLY);
  if (fd_ == -1) {
    throw std::invalid_argument("file open error");
  }
  fcntl(fd_, F_SETFL, O_NONBLOCK);
  fcntl(fd_, F_SETFD, FD_CLOEXEC);
  struct kevent event;
  EV_SET(&event, fd_, EVFILT_READ, EV_ADD, 0, 0, this);
  kevent(kq_, &event, 1, NULL, 0, 0);
}

EventController::returnType FileReadEventController::handleEvent(
    const struct kevent &event) {
  if (event.filter != EVFILT_READ) {
    std::cout << "unexpected event" << std::endl;
    close(fd_);
    return FAIL;
  }
  content_.reserve(event.data);
  char buffer[FILE_READ_BUFF_SIZE];
  intptr_t i = 0;
  while (i < event.data) {
    int size = read(fd_, buffer, FILE_READ_BUFF_SIZE);
    content_.append(buffer, size);
    i += size;
  }
  close(fd_);
  if (observer_) {
    observer_->fileEvent(FILE_READ | FILE_SUCCESS, content_);
  }
  return SUCCESS;
}
