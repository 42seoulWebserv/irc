#include "CgiInProcessor.hpp"

#include "CgiEventController.hpp"
#include "CgiOutProcessor.hpp"
#include "ErrorPageProcessor.hpp"

CgiInProcessor::CgiInProcessor(IClient &client)
    : client_(client), cgi_(NULL), error_(false), cgiRequestEnd_(false) {}

struct ProcessResult CgiInProcessor::process() {
  if (cgi_ == NULL) {
    cgi_ = CgiEventController::addEventController(client_.getRequest());
    if (cgi_ == NULL) {
      client_.setResponseStatusCode(500);
      return ProcessResult().setNextProcessor(new ErrorPageProcessor(client_));
    }
    cgi_->setCgiObserver(this);
  }
  if (error_) {
    client_.setResponseStatusCode(500);
    return ProcessResult().setNextProcessor(new ErrorPageProcessor(client_));
  }
  if (cgiRequestEnd_) {
    CgiOutProcessor *out = new CgiOutProcessor(client_);
    cgi_->setCgiObserver(out);
    return ProcessResult().setNextProcessor(out);
  }
  return ProcessResult();
}

void CgiInProcessor::onEvent(const IObserver<CgiEventController::Event> &p) {
  (void)p;
}
