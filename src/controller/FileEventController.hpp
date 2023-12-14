#ifndef FileEventController_HPP_
#define FileEventController_HPP_

#include <fcntl.h>
#include <unistd.h>

#include "DataStream.hpp"
#include "EventController.hpp"
#include "FilePath.hpp"
#include "ICancelable.hpp"
#include "IObserver.hpp"

#define FILE_ERROR_NOT_SUPPORT_MODE "file: not support mode"
#define FILE_ERROR_OPEN_FILE "file: open file error"

class FileEventController : public EventController, public ICancelable {
 public:
  struct Event {
    Event() : error_(false), end_(false){};
    Event &setError(bool error);
    Event &setMode(int mode);
    Event &setEnd(bool end);
    Event &setTotal(long total);
    int mode_;
    bool error_;
    bool end_;
    long total_;
  };

  ~FileEventController();
  static FileEventController *addEventController(FilePath path,
                                                 DataStream &buffer, int mode,
                                                 IObserver<Event> *observer);

  void init();
  void handleEvent(const Multiplexer::Event &event);
  void cancel();

 private:
  int mode_;
  bool isOpen_;
  FilePath path_;
  DataStream &buffer_;
  IObserver<Event> *observer_;
  long startPosition_;

  FileEventController(FilePath path, DataStream &buffer, int mode,
                      IObserver<Event> *observer);
  void notify(struct Event &event);
};

#endif
