#include "SelectMethodProcessor.hpp"

#include "CgiProcessor.hpp"
#include "ErrorPageProcessor.hpp"
#include "MethodDeleteProcessor.hpp"
#include "MethodGetProcessor.hpp"
#include "MethodPostProcessor.hpp"

SelectMethodProcessor::SelectMethodProcessor(IClient& client)
    : client_(client), response_(client.getResponse()) {}

ProcessResult SelectMethodProcessor::process() {
  ProcessResult res;
  const std::string& method = client_.getRequest().getMethod();
  const std::vector<std::string>& accepts =
      client_.getLocationConfig()->getAcceptMethods();
  if (client_.getRequest().hasHeader("Connection")) {
    const std::string& conn = client_.getRequest().getHeader("Connection");
    response_.setHeader("Connection", conn);
    res.setResponse(&response_);
  }
  if (client_.getRequest().getVersion() != "HTTP/1.1") {
    return res.setStatus(505).setNextProcessor(new ErrorPageProcessor(client_));
  }
  if (std::find(accepts.begin(), accepts.end(), method) == accepts.end()) {
    return res.setStatus(405).setNextProcessor(new ErrorPageProcessor(client_));
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
  return res.setStatus(500).setNextProcessor(new ErrorPageProcessor(client_));
}
