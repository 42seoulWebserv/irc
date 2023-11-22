#ifndef FileReadEventController_HPP_
#define FileReadEventController_HPP_

#define FILE_READ_BUFF_SIZE 16384

#include <cstdlib>

#include "DataStream.hpp"
#include "EventController.hpp"
#include "FilePath.hpp"
#include "ICancelible.hpp"
#include "IObserver.hpp"
#include "KqueueMultiplexer.hpp"

class FileReadEventController : public EventController, public ICancelible {
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
  void cancel();

 private:
  FileReadEventController(const std::string &filepath,
                          IObserver<Event> *observer, DataStream *stream);
  FILE *file_;
  int fd_;
  FilePath filepath_;
  int totalReadSize_;
  IObserver<Event> *observer_;
  DataStream *dataStream_;
  bool isCanceled_;
};

#endif
