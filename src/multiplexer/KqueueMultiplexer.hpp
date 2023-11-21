#ifndef KqueueMultiplexer_HPP_
#define KqueueMultiplexer_HPP_

#include <sys/event.h>
#include <sys/socket.h>
#include <sys/types.h>

#define TIMEOUT 10

class KqueueMultiplexer {
 public:
  static KqueueMultiplexer &getInstance();
  void addReadEvent(int fd, void *udata);
  void addReadEventWithClearFlag(int fd, void *udata);
  void addWriteEvent(int fd, void *udata);
  void removeReadEvent(int fd, void *udata);
  void removeWriteEvent(int fd, void *udata);
  void addTimeoutEvent(int fd, void *udata);
  void removeTimeoutEvent(int fd, void *udata);
  int getKq();

 private:
  KqueueMultiplexer();
  KqueueMultiplexer(const KqueueMultiplexer &rhs);
  KqueueMultiplexer &operator=(const KqueueMultiplexer &rhs);
  int kq_;
};

#endif
