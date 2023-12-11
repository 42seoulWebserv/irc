#include "EventController.hpp"

EventController::EventController(IProcessor *processor)
    : deprecated_(false), processor_(processor) {}

EventController::~EventController() { delete processor_; }

int EventController::getFd() const { return fd_; }

bool EventController::isDeprecated() const { return deprecated_; }

void EventController::setDeprecated(bool deprecated) {
  deprecated_ = deprecated;
}

bool EventController::loopProcess() {
  ProcessResult processResult = nextProcessor();
  while (processResult.nextProcessor_ && processResult.error_ == false) {
    processResult = nextProcessor();
  }
  return processResult.error_;
}

void EventController::reserveDeleteSelf() {
  Multiplexer::getInstance().addDeleteController(this);
}

void EventController::setProcessor(IProcessor *processor) {
  processor_ = processor;
}

IProcessor *EventController::getProcessor() const { return processor_; }

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
  if (res.nextProcessor_) {
    delete processor_;
    processor_ = res.nextProcessor_;
  }
  return res;
}
