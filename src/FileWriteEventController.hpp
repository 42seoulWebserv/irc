#ifndef FileWriteEventController_HPP_
#define FileWriteEventController_HPP_

#define FILE_READ_BUFF_SIZE 16384

#include "EventController.hpp"
#include "IFileObserver.hpp"

class FileWriteEventController : public EventController {
 public:
  FileWriteEventController(int kq, const std::string &filepath,
                           const std::string &content, IFileObserver *observer);
  enum EventController::returnType handleEvent(const struct kevent &event);

 private:
  int kq_;
  int fd_;
  std::string filepath_;
  std::string content_;
  size_t offset_;
  IFileObserver *observer_;
};

#endif
