#include "ProcessResult.hpp"

ProcessResult::ProcessResult()
    : writeOn_(false),
      writeOff_(false),
      readOn_(false),
      readOff_(false),
      error_(false),
      nextProcessor_(NULL) {}

ProcessResult& ProcessResult::setWriteOn(bool b) {
  this->writeOn_ = b;
  return *this;
}

ProcessResult& ProcessResult::setWriteOff(bool b) {
  this->writeOff_ = b;
  return *this;
}

ProcessResult& ProcessResult::setReadOn(bool b) {
  this->readOn_ = b;
  return *this;
}

ProcessResult& ProcessResult::setReadOff(bool b) {
  this->readOff_ = b;
  return *this;
}

ProcessResult& ProcessResult::setError(bool b) {
  error_ = b;
  return *this;
}

ProcessResult& ProcessResult::setNextProcessor(
    IRequestProcessor* nextProcessor) {
  this->nextProcessor_ = nextProcessor;
  return *this;
}
