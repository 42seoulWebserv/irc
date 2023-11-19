#include "ProcessResult.hpp"

ProcessResult::ProcessResult()
    : writeOn_(false),
      writeOff_(false),
      readOn_(false),
      readOff_(false),
      error_(false),
      spendReadBuffer_(0),
      status_(0),
      nextProcessor_(nullptr),
      request(nullptr),
      response(nullptr) {}

ProcessResult& ProcessResult::setWriteOn(bool b) {
  writeOn_ = b;
  return *this;
}

ProcessResult& ProcessResult::setWriteOff(bool b) {
  writeOff_ = b;
  return *this;
}

ProcessResult& ProcessResult::setReadOn(bool b) {
  readOn_ = b;
  return *this;
}

ProcessResult& ProcessResult::setReadOff(bool b) {
  readOff_ = b;
  return *this;
}

ProcessResult& ProcessResult::setError(bool b) {
  error_ = b;
  return *this;
}

ProcessResult& ProcessResult::setSpendReadBuffer(unsigned spendReadBuffer) {
  this->spendReadBuffer_ = spendReadBuffer;
  return *this;
}

ProcessResult& ProcessResult::setStatus(int status) {
  this->status_ = status;
  return *this;
}

ProcessResult& ProcessResult::setNextProcessor(
    IRequestProcessor* nextProcessor) {
  this->nextProcessor_ = nextProcessor;
  return *this;
}

ProcessResult& ProcessResult::setRequest(const Request* request) {
  this->request = request;
  return *this;
}

ProcessResult& ProcessResult::setResponse(const Response* response) {
  this->response = response;
  return *this;
}
