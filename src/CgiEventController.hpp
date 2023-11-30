#ifndef CgiEventController_HPP_
#define CgiEventController_HPP_

#include "DataStream.hpp"
#include "EventController.hpp"
#include "ICancelable.hpp"
#include "ICgi.hpp"
#include "IClient.hpp"
#include "IObserver.hpp"
#include "Request.hpp"

class CgiEventController : public EventController,
                           public ICgi,
                           public ICancelable {
 public:
  struct Event {
    // TODO
  };

  ~CgiEventController();
  void init();

  static CgiEventController *addEventController(
      IClient &client, IObserver<CgiEventController::Event> *observer);
  void handleEvent(const Multiplexer::Event &event);
  void cancel();

  void setFd(int &fd);
  int getFd();

 private:
  CgiEventController(IClient &client,
                     IObserver<CgiEventController::Event> *observer);
  IClient &client_;
  bool cancel_;
  pid_t pid_;
  IObserver<CgiEventController::Event> *observer_;
  int fd_;
};

#endif
