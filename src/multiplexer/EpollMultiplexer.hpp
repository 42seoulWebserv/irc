#ifndef EpollMultiplexer_HPP_
#define EpollMultiplexer_HPP_

class EpollMultiplexer {
 public:
  static EpollMultiplexer &getInstance();
  void addReadEvent(int fd, void *udata);
  void addReadEventWithClearFlag(int fd, void *udata);
  void addWriteEvent(int fd, void *udata);
  void removeReadEvent(int fd, void *udata);
  void removeWriteEvent(int fd, void *udata);
  void addTimeoutEvent(int fd, void *udata);
  void removeTimeoutEvent(int fd, void *udata);
  int getEp();

 private:
  EpollMultiplexer();
  EpollMultiplexer(const EpollMultiplexer &rhs);
  EpollMultiplexer &operator=(const EpollMultiplexer &rhs);
  int ep_;
};

#endif
