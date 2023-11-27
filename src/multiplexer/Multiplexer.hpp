#ifndef Multiplexer_HPP_
#define Multiplexer_HPP_

#include <vector>
#define TIMEOUT 10
#define MAX_BUFFER_SIZE 16384

#define WEB_READ 1
#define WEB_WRITE 2
#define WEB_TIMEOUT 4

class EventController;
class Multiplexer {
 public:
  struct Event {
    int filter;
    EventController *controller;
  };

  static Multiplexer &getInstance();
  void addReadEvent(int fd, void *controller);
  void addWriteEvent(int fd, void *controller);
  void removeReadEvent(int fd, void *controller);
  void removeWriteEvent(int fd, void *controller);
  void addTimeoutEvent(int fd, void *controller);
  void removeTimeoutEvent(int fd, void *controller);
  std::vector<Event> wait(int size);

 private:
  Multiplexer();
  Multiplexer(const Multiplexer &rhs);
  Multiplexer &operator=(const Multiplexer &rhs);
  int fd_;
};

#endif
