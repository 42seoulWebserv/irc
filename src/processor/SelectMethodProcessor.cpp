#include "SelectMethodProcessor.hpp"

#include <algorithm>

#include "CgiProcessor.hpp"
#include "ErrorPageProcessor.hpp"
#include "FilePath.hpp"
#include "MethodDeleteProcessor.hpp"
#include "MethodGetProcessor.hpp"
#include "MethodPostProcessor.hpp"
#include "MethodPutProcessor.hpp"
#include "RedirectionProcessor.hpp"

SelectMethodProcessor::SelectMethodProcessor(IClient& client)
    : client_(client) {
  client_.print(Log::info, " SelectMethodProcessor");
}

static bool cgiChecker(IClient& client_) {
  FilePath extension = FilePath::getExtension(client_.getRequest().getUri());
  if (!client_.getLocationConfig()->hasCgiProgram('.' + extension)) {
    return false;
  }
  return true;
}

ProcessResult SelectMethodProcessor::process() {
  ProcessResult res;
  if (client_.getLocationConfig() == NULL) {
    client_.setResponseStatusCode(503);
    return res.setNextProcessor(new ErrorPageProcessor(client_));
  }
  const std::string& method = client_.getRequest().getMethod();
  const std::vector<std::string>& accepts =
      client_.getLocationConfig()->getAcceptMethods();
  if (client_.getRequest().hasHeader("Connection")) {
    const std::string& conn = client_.getRequest().getHeader("Connection");
    client_.setResponseHeader("Connection", conn);
  }
  if (client_.getRequest().getVersion() != "HTTP/1.1") {
    client_.setResponseStatusCode(505);
    return res.setNextProcessor(new ErrorPageProcessor(client_));
  }
  if (cgiChecker(client_)) {
    return ProcessResult().setWriteOn(true).setNextProcessor(
        new CgiProcessor(client_));
  }
  if (client_.getLocationConfig()->getRedirectionStatusCode()) {
    int code = client_.getLocationConfig()->getRedirectionStatusCode();
    client_.setResponseStatusCode(code);
    return res.setNextProcessor(new RedirectionProcessor(client_));
  }
  if (std::find(accepts.begin(), accepts.end(), method) == accepts.end()) {
    client_.setResponseStatusCode(405);
    return res.setNextProcessor(new ErrorPageProcessor(client_));
  }
  if (method == "GET") {
    return res.setNextProcessor(new MethodGetProcessor(client_));
  }
  if (method == "POST") {
    return res.setNextProcessor(new MethodPostProcessor(client_));
  }
  if (method == "DELETE") {
    return res.setNextProcessor(new MethodDeleteProcessor(client_));
  }
  if (method == "PUT") {
    return res.setNextProcessor(new MethodPutProcessor(client_));
  }
  client_.setResponseStatusCode(500);
  return res.setNextProcessor(new ErrorPageProcessor(client_));
}
