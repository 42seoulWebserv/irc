#include "SelectMethodProcessor.hpp"

#include <algorithm>

#include "CgiProcessor.hpp"
#include "ErrorPageProcessor.hpp"
#include "MethodDeleteProcessor.hpp"
#include "MethodGetProcessor.hpp"
#include "MethodPostProcessor.hpp"
#include "RedirectionProcessor.hpp"

SelectMethodProcessor::SelectMethodProcessor(IClient& client)
    : client_(client) {}

ProcessResult SelectMethodProcessor::process() {
  ProcessResult res;
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
  client_.setResponseStatusCode(500);
  return res.setNextProcessor(new ErrorPageProcessor(client_));
}
