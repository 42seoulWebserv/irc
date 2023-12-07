#include "RedirectionProcessor.hpp"

#include "WaitProcessor.hpp"

RedirectionProcessor::RedirectionProcessor(IClient& client) : client_(client) {
  client_.print(Log::info, " RedirectionProcessor");
}

ProcessResult RedirectionProcessor::process() {
  int status = client_.getLocationConfig()->getRedirectionStatusCode();
  std::string path = client_.getLocationConfig()->getRedirectionPath();
  client_.setResponseStatusCode(status);
  client_.setResponseHeader("Location", path);
  client_.setResponseHeader("Content-Length", "0");
  client_.getDataStream().push(client_.getResponse().toString());
  client_.getDataStream().markEOF();
  return ProcessResult().setWriteOn(true).setNextProcessor(new WaitProcessor());
}
