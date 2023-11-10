#ifndef FileReadEventController_HPP_
#define FileReadEventController_HPP_

#define FILE_READ_BUFF_SIZE 16384

#include "EventController.hpp"
#include "IObserver.hpp"

class FileReadEventController : public EventController {
 public:
  enum EventType { SUCCESS, FAIL };
  class Event {
   public:
    Event(enum EventType type, const std::string &content);
    enum EventType type_;
    const std::string &content_;
  };
  static void addEventController(int kq_, const std::string &filepath,
                                 IObserver<Event> *observer);
  enum EventController::returnType handleEvent(const struct kevent &event);

 private:
  FileReadEventController(int kq, const std::string &filepath,
                          IObserver<Event> *observer);
  int kq_;
  int fd_;
  std::string filepath_;
  std::string content_;
  IObserver<Event> *observer_;
};

#endif
