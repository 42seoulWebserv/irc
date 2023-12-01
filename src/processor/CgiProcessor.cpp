#include "CgiProcessor.hpp"

#include <sstream>

#include "ErrorPageProcessor.hpp"
#include "WaitProcessor.hpp"

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
    std::stringstream ss;
    ss << client_.getBody().size();
    client_.setResponseHeader("Content-Length", ss.str());
    client_.getDataStream().readStr(client_.getResponse().toString());
    client_.getDataStream().readStr(client_.getBody());
    client_.getDataStream().setEof(true);

    return ProcessResult().setNextProcessor(new WaitProcessor());
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
  error_ = e.error_;
}
