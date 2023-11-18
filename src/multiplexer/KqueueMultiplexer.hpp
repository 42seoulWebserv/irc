#ifndef KqueueMultiplexer_HPP_
#define KqueueMultiplexer_HPP_

#include <sys/event.h>
#include <sys/socket.h>
#include <sys/types.h>

class KqueueMultiplexer {
 public:
  static KqueueMultiplexer &getInstance() {
    static KqueueMultiplexer instance;
    return instance;
  };
  void addReadEvent(int fd, void *udata);
  void addReadEventWithClearFlag(int fd, void *udata);
  void addWriteEvent(int fd, void *udata);
  void removeReadEvent(int fd, void *udata);
  void removeWriteEvent(int fd, void *udata);
  int getKq();

 private:
  KqueueMultiplexer();
  KqueueMultiplexer(const KqueueMultiplexer &rhs);
  KqueueMultiplexer &operator=(const KqueueMultiplexer &rhs);
  int kq_;
};

#endif