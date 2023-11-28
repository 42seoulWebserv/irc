#ifndef CgiEventController_HPP_
#define CgiEventController_HPP_

#include "DataStream.hpp"
#include "EventController.hpp"
#include "Request.hpp"

class CgiEventController : public EventController {
 public:
  struct Event {
    // TODO
  };

  static CgiEventController *addEventController(const Request &req);
  enum returnType handleEvent(const Multiplexer::Event &event);
  void appendRequestStr(const std::string &str);
  void setCgiObserver(IObserver<CgiEventController::Event> *observer);

 private:
  CgiEventController();
  DataStream data_;
  IObserver<CgiEventController::Event> *observer_;
};

#endif
