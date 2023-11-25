#ifndef SERVEREVENTCONTROLLER_HPP_
#define SERVEREVENTCONTROLLER_HPP_

#include "ClientEventController.hpp"
#include "EventController.hpp"

class ServerEventController : public EventController {
 public:
  ServerEventController(int port);
  ~ServerEventController();

  enum EventController::returnType handleEvent(const Multiplexer::Event& event);

 private:
  int port_;
};

#endif
