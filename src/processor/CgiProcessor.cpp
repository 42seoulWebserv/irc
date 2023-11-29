#include "CgiProcessor.hpp"

#include "ErrorPageProcessor.hpp"

CgiProcessor::CgiProcessor(IClient& client)
    : client_(client), cgi_(NULL), error_(false), end_(false) {}

CgiProcessor::~CgiProcessor() {
  if (cgi_) {
    cgi_->cancel();
  }
}

ProcessResult CgiProcessor::process() {
  if (error_) {
    client_.setResponseStatusCode(500);
    return ProcessResult().setNextProcessor(new ErrorPageProcessor(client_));
  }
  if (end_) {
    return ProcessResult();  // TODO
  }
  if (cgi_) {
    return ProcessResult();
  }
  cgi_ = CgiEventController::addEventController(client_, this);
  if (cgi_ == NULL) {
    client_.setResponseStatusCode(500);
    return ProcessResult().setNextProcessor(new ErrorPageProcessor(client_));
  }
  return ProcessResult();
}

void CgiProcessor::onEvent(const CgiEventController::Event& e) {
  cgi_ = NULL;
  end_ = true;
}
