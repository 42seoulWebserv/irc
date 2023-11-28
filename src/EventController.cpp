#include "EventController.hpp"

EventController::EventController(IProcessor *processor)
    : processor_(processor) {}

EventController::~EventController() { delete processor_; }

int EventController::getFd() const { return fd_; }

bool EventController::loopProcess() {
  ProcessResult processResult = nextProcessor();
  while (processResult.nextProcessor_ && processResult.error_ == false) {
    processResult = nextProcessor();
  }
  return processResult.error_;
}

ProcessResult EventController::nextProcessor() {
  if (processor_ == NULL) {
    return ProcessResult().setError(true);
  }
  ProcessResult res = processor_->process();
  if (res.error_) {
    return res;
  }
  if (res.readOn_) {
    Multiplexer::getInstance().addReadEvent(fd_, this);
  }
  if (res.readOff_) {
    Multiplexer::getInstance().removeReadEvent(fd_, this);
  }
  if (res.writeOn_) {
    Multiplexer::getInstance().addWriteEvent(fd_, this);
  }
  if (res.writeOff_) {
    Multiplexer::getInstance().removeWriteEvent(fd_, this);
  }
  if (res.spendReadBuffer_) {
    spendBuffer(res.spendReadBuffer_);
  }
  if (res.nextProcessor_) {
    delete processor_;
    processor_ = res.nextProcessor_;
  }
  return res;
}
