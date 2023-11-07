#ifndef FileReadEventController_HPP_
#define FileReadEventController_HPP_

#define FILE_READ_BUFF_SIZE 16384

#include "EventController.hpp"
#include "IFileObserver.hpp"

class FileReadEventController : public EventController{
public:
  FileReadEventController(int kq, const std::string &filepath, IFileObserver *observer);
  enum EventController::returnType handleEvent(const struct kevent &event);

private:
  int kq_;
  int fd_;
  std::string filepath_;
  std::string content_;
  IFileObserver *observer_;

};

#endif
