#ifndef SERVEREVENTCONTROLLER_HPP_
#define SERVEREVENTCONTROLLER_HPP_

#include "ClientEventController.hpp"
#include "EventController.hpp"

class ServerEventController : public EventController {
 public:
  ServerEventController(int kq, int port);
  ~ServerEventController();

  enum EventController::returnType handleEvent(const struct kevent &event);

 private:
  int kq_;
  int socket_;
  int port_;
};

#endif
