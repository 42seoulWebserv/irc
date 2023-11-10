#include "FileReadEventController.hpp"

#include <fcntl.h>
#include <sys/event.h>  // kevent
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <exception>
#include <fstream>

FileReadEventController::FileReadEventController(int kq,
                                                 const std::string &filepath,
                                                 IObserver<Event> *observer)
    : kq_(kq), filepath_(filepath), observer_(observer) {
  const char *file = filepath.c_str();
  struct stat statbuf;
  if (stat(file, &statbuf)) {
    throw FileReadException(FileReadEventController::NOT_FOUND);
  }
  if (access(file, R_OK)) {
    throw FileReadException(FileReadEventController::NOT_ACCESS);
  }
  if (statbuf.st_size == 0) {
    throw FileReadException(FileReadEventController::SUCCESS);
  }
  fd_ = open(file, O_RDONLY);
  if (fd_ == -1) {
    throw FileReadException(FileReadEventController::FAIL);
  }
  fcntl(fd_, F_SETFL, O_NONBLOCK);
  fcntl(fd_, F_SETFD, FD_CLOEXEC);
  struct kevent event;
  EV_SET(&event, fd_, EVFILT_READ, EV_ADD, 0, 0, this);
  kevent(kq_, &event, 1, NULL, 0, 0);
}

void FileReadEventController::addEventController(int kq_,
                                                 const std::string &filepath,
                                                 IObserver<Event> *observer) {
  try {
    new FileReadEventController(kq_, filepath, observer);
  } catch (const FileReadException &e) {
    if (observer == NULL) {
      return;
    }
    observer->onEvent(Event(e.type_, ""));
  } catch (...) {
    observer->onEvent(Event(FileReadEventController::FAIL, ""));
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

FileReadEventController::FileReadException::FileReadException(EventType type)
    : type_(type) {}