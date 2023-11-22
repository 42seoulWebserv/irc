#include "RedirectionProcessor.hpp"

#include "WaitProcessor.hpp"

RedirectionProcessor::RedirectionProcessor(IClient& client) : client_(client) {}

ProcessResult RedirectionProcessor::process() {
  int status = client_.getLocationConfig()->getRedirectionStatusCode();
  std::string path = client_.getLocationConfig()->getRedirectionPath();
  response_ = client_.getResponse();
  response_.setStatusCode(status);
  response_.setHeader("Location", path);
  response_.setHeader("Content-Length", "0");
  client_.getDataStream().readStr(response_.toString());
  client_.getDataStream().setEof(true);
  return ProcessResult()
      .setWriteOn(true)
      .setResponse(&response_)
      .setNextProcessor(new WaitProcessor());
  // TODO : redirection을 위한 processor 만들기.
}
