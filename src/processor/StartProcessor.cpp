#include "StartProcessor.hpp"

#include "ParseRequestProcessor.hpp"

StartProcessor::StartProcessor(IClient& client) : client_(client) {}

ProcessResult StartProcessor::process() {
  Response response = client_.getResponse();
  response.setVersion("HTTP/1.1");
  response.setStatusCode(100);
  response.setHeader("Server", "irc");
  response.setHeader("Connection", "keep-alive");
  response.setHeader("Content-Type", "text/html");
  client_.setResponse(response);
  return ProcessResult().setNextProcessor(new ParseRequestProcessor(client_));
};
