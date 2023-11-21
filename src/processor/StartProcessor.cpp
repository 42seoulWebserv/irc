#include "StartProcessor.hpp"

#include "ParseRequestProcessor.hpp"

StartProcessor::StartProcessor(IClient& client)
    : client_(client), response_(client.getResponse()) {}

ProcessResult StartProcessor::process() {
  response_.setVersion("HTTP/1.1");
  response_.setStatusCode(100);
  response_.setHeader("Server", "irc");
  response_.setHeader("Connection", "keep-alive");
  response_.setHeader("Content-Type", "text/html");
  return ProcessResult()
      .setResponse(&response_)
      .setNextProcessor(new ParseRequestProcessor(client_));
};
