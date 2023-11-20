#include "SelectMethodProcessor.hpp"

#include "CgiProcessor.hpp"
#include "ErrorPageProcessor.hpp"
#include "MethodDeleteProcessor.hpp"
#include "MethodGetProcessor.hpp"
#include "MethodPostProcessor.hpp"

SelectMethodProcessor::SelectMethodProcessor(IClient& client)
    : client_(client) {}

ProcessResult SelectMethodProcessor::process() {
  ProcessResult res;
  const std::string& method = client_.getRequest().getMethod();
  const std::vector<std::string>& accepts =
      client_.getLocationConfig()->getAcceptMethods();
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
