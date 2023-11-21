#ifndef FileReadEventController_HPP_
#define FileReadEventController_HPP_

#define FILE_READ_BUFF_SIZE 16384

#include <cstdlib>

#include "DataStream.hpp"
#include "EventController.hpp"
#include "IObserver.hpp"
#include "KqueueMultiplexer.hpp"

class FileReadEventController : public EventController {
 public:
  enum EventType { SUCCESS, FAIL };
  class Event {
   public:
    Event(enum EventType type);
    enum EventType type_;
  };
  static FileReadEventController *addEventController(
      const std::string &filepath, IObserver<Event> *observer,
      DataStream *stream);
  enum EventController::returnType handleEvent(const struct kevent &event);

 private:
  FileReadEventController(const std::string &filepath,
                          IObserver<Event> *observer, DataStream *stream);
  FILE *file_;
  int fd_;
  std::string filepath_;
  std::string content_;
  IObserver<Event> *observer_;
  DataStream *dataStream_;
};

#endif
