#ifndef EVENTCONTROLLER_HPP_
#define EVENTCONTROLLER_HPP_

#include "IProcessor.hpp"
#include "Multiplexer.hpp"
#include "ProcessResult.hpp"
#include "RootConfig.hpp"

/* event hadling
** kqueue가 발생시키는 event를 handle
*/
class EventController {
 public:
  EventController(IProcessor *processor);
  virtual ~EventController();

  virtual void init() = 0;
  virtual void handleEvent(const Multiplexer::Event &event) = 0;
  virtual void spendBuffer(int size) = 0;

  int getFd() const;

 protected:
  int fd_;
  bool loopProcess();
  void reserveDeleteSelf();

 private:
  IProcessor *processor_;
  ProcessResult nextProcessor();
};

#endif
