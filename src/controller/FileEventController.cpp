#include "FileEventController.hpp"

#include "Log.hpp"
#include "WaitProcessor.hpp"

FileEventController::~FileEventController() {
  if (isOpen_) {
    close(fd_);
    Log::debug << fd_ << ": regular file closed" << NL;
  }
}

FileEventController* FileEventController::addEventController(
    FilePath path, DataStream& buffer, int mode, IObserver<Event>* observer) {
  FileEventController* controller =
      new FileEventController(path, buffer, mode, observer);
  if (controller == NULL) {
    return NULL;
  }
  try {
    controller->init();
  } catch (...) {
    delete controller;
    return NULL;
  }
  if (mode == O_RDONLY) {
    Multiplexer::getInstance().addReadEvent(controller->fd_, controller);
  } else {
    Multiplexer::getInstance().addWriteEvent(controller->fd_, controller);
  }
  return controller;
}

FileEventController::FileEventController(FilePath path, DataStream& buffer,
                                         int mode, IObserver<Event>* observer)
    : EventController(new WaitProcessor()),
      mode_(mode),
      isOpen_(false),
      path_(path),
      buffer_(buffer),
      observer_(observer),
      startPosition_(0) {}

void FileEventController::init() {
  if (mode_ == O_RDONLY) {
    fd_ = open(path_.getPath().c_str(), O_RDONLY);
  } else if (mode_ == O_WRONLY) {
    fd_ = open(path_.getPath().c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
  } else {
    throw std::runtime_error(FILE_ERROR_NOT_SUPPORT_MODE);
  }
  if (fd_ == -1) {
    throw std::runtime_error(FILE_ERROR_OPEN_FILE);
  }
  isOpen_ = true;
  if (mode_ == O_RDONLY) {
    startPosition_ = buffer_.getTotalRead();
  } else if (mode_ == O_WRONLY) {
    startPosition_ = buffer_.getTotalWrite();
  }
  Log::debug << fd_ << ": regular file opened" << NL;
}

void FileEventController::handleEvent(const Multiplexer::Event& event) {
  if (event.filter == WEB_READ && mode_ == O_RDONLY) {
    int size = buffer_.push(fd_);
    if (size == -1) {
      notify(Event().setError(true));
      Multiplexer::getInstance().addDeleteController(this);
      return;
    }
    if (size == 0) {
      notify(Event().setError(false));
      Multiplexer::getInstance().addDeleteController(this);
      return;
    }
  }
  if (event.filter == WEB_WRITE && mode_ == O_WRONLY) {
    int size = buffer_.popToFile(fd_);
    if (size == -1) {
      notify(Event().setError(true));
      Multiplexer::getInstance().addDeleteController(this);
      return;
    }
    if (size == 0) {
      notify(Event().setError(false));
      Multiplexer::getInstance().addDeleteController(this);
      return;
    }
  }
}

void FileEventController::cancel() {
  Multiplexer::getInstance().addDeleteController(this);
}

void FileEventController::notify(Event& event) {
  if (observer_) {
    long endPosition = 0;
    if (mode_ == O_RDONLY) {
      endPosition = buffer_.getTotalRead();
    } else if (mode_ == O_WRONLY) {
      endPosition = buffer_.getTotalWrite();
    }
    long total = endPosition - startPosition_;
    observer_->onEvent(event.setEnd(true).setTotal(total));
  }
}

FileEventController::Event& FileEventController::Event::setError(bool error) {
  error_ = error;
  return *this;
}

FileEventController::Event& FileEventController::Event::setMode(int mode) {
  mode_ = mode;
  return *this;
}

FileEventController::Event& FileEventController::Event::setEnd(bool end) {
  end_ = end;
  return *this;
}

FileEventController::Event& FileEventController::Event::setTotal(long total) {
  total_ = total;
  return *this;
}
