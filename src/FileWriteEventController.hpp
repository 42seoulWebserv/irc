#ifndef FileWriteEventController_HPP_
#define FileWriteEventController_HPP_

#define FILE_READ_BUFF_SIZE 16384

#include "EventController.hpp"
#include "IObserver.hpp"

class FileWriteEventController : public EventController {
 public:
  enum EventType { SUCCESS, FAIL };
  class Event {
   public:
    Event(enum EventType type);
    enum EventType type_;
  };

  static void addEventController(const std::string &filepath,
                                 const std::string &content,
                                 IObserver<Event> *observer);

  enum EventController::returnType handleEvent(const struct kevent &event);

 private:
  FileWriteEventController(const std::string &filepath,
                           const std::string &content,
                           IObserver<Event> *observer);
  int fd_;
  std::string filepath_;
  std::string content_;
  size_t offset_;
  IObserver<Event> *observer_;
};

#endif
