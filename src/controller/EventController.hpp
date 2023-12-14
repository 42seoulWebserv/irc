#ifndef EVENTCONTROLLER_HPP_
#define EVENTCONTROLLER_HPP_

#include "IProcessor.hpp"
#include "Multiplexer.hpp"
#include "ProcessResult.hpp"

class EventController {
 public:
  EventController(IProcessor *processor);
  virtual ~EventController();

  virtual void init() = 0;
  virtual void handleEvent(const Multiplexer::Event &event) = 0;

  int getFd() const;
  bool isDeprecated() const;
  void setDeprecated(bool deprecated);

 protected:
  int fd_;
  bool deprecated_;
  bool loopProcess();
  void reserveDeleteSelf();
  void setProcessor(IProcessor *processor);
  IProcessor *getProcessor() const;

 private:
  IProcessor *processor_;
  ProcessResult nextProcessor();
};

#endif
