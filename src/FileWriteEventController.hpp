#ifndef FileWriteEventController_HPP_
#define FileWriteEventController_HPP_

#define FILE_READ_BUFF_SIZE 16384

#include "EventController.hpp"
#include "IObserver.hpp"

class FileWriteEventController : public EventController {
 public:
  enum EventType { SUCCESS, FAIL, NOT_ACCESS };
  class Event {
   public:
    Event(enum EventType type);
    enum EventType type_;
  };
  class FileWriteException : public std::exception {
   public:
    FileWriteException(EventType type);
    enum EventType type_;
  };
  static void addEventController(int kq, const std::string &filepath,
                                 const std::string &content,
                                 IObserver<Event> *observer);

  enum EventController::returnType handleEvent(const struct kevent &event);

 private:
  FileWriteEventController(int kq, const std::string &filepath,
                           const std::string &content,
                           IObserver<Event> *observer);
  int kq_;
  int fd_;
  std::string filepath_;
  std::string content_;
  size_t offset_;
  IObserver<Event> *observer_;
};

#endif
