#ifndef Multiplexer_HPP_
#define Multiplexer_HPP_

#include <set>
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
  void addReadEvent(int fd, EventController *controller);
  void addWriteEvent(int fd, EventController *controller);
  void removeReadEvent(int fd, EventController *controller);
  void removeWriteEvent(int fd, EventController *controller);
  void addTimeoutEvent(int fd, EventController *controller);
  void removeTimeoutEvent(int fd, EventController *controller);
  void addDeleteController(EventController *controller);
  void deleteAddedControllers();
  std::vector<Event> wait(int size);

 private:
  Multiplexer();
  Multiplexer(const Multiplexer &rhs);
  Multiplexer &operator=(const Multiplexer &rhs);
  std::set<EventController *> deleteList_;
  int fd_;
};

#endif
