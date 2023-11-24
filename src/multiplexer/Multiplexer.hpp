#ifndef Multiplexer_HPP_
#define Multiplexer_HPP_

#include <vector>
#define TIMEOUT 10
#define MAX_BUFFER_SIZE 16384

#define WEB_READ 1
#define WEB_WRITE 2
#define WEB_TIMEOUT 4

class Multiplexer {
 public:
  struct Event {
    int ident;
    int filter;
    void *udata;
  };

  static Multiplexer &getInstance();
  void addReadEvent(int fd, void *udata);
  void addReadEventWithClearFlag(int fd, void *udata);
  void addWriteEvent(int fd, void *udata);
  void removeReadEvent(int fd, void *udata);
  void removeWriteEvent(int fd, void *udata);
  void addTimeoutEvent(int fd, void *udata);
  void removeTimeoutEvent(int fd, void *udata);
  std::vector<Event> wait(int size);

 private:
  Multiplexer();
  Multiplexer(const Multiplexer &rhs);
  Multiplexer &operator=(const Multiplexer &rhs);
  int fd_;
};

#endif
