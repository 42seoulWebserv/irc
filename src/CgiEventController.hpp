#ifndef CgiEventController_HPP_
#define CgiEventController_HPP_

#include "DataStream.hpp"
#include "EventController.hpp"
#include "ICancelable.hpp"
#include "ICgi.hpp"
#include "IClient.hpp"
#include "IObserver.hpp"
#include "Request.hpp"
#include "StringBuffer.hpp"

class CgiEventController : public EventController,
                           public ICgi,
                           public ICancelable {
 public:
  struct Event {
    Event() : error_(false){};
    Event &setError(bool error);
    bool error_;
  };

  ~CgiEventController();
  void init();

  static CgiEventController *addEventController(
      IClient &client, IObserver<CgiEventController::Event> *observer);
  void handleEvent(const Multiplexer::Event &event);
  void cancel();

  void setFd(int &fd);
  int getFd();
  StringBuffer &getRecvBuffer();
  void end();
  DataStream &getWriteBuffer();
  void clear(bool error);

 private:
  CgiEventController(IClient &client,
                     IObserver<CgiEventController::Event> *observer);
  IClient &client_;
  bool cancel_;
  pid_t pid_;
  IObserver<CgiEventController::Event> *observer_;
  StringBuffer recvBuffer_;
  bool end_;
  DataStream writeBuffer_;
};

#endif
