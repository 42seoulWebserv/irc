#include "ErrorPageProcessor.hpp"

#include <sstream>

#include "CgiProcessor.hpp"
#include "MethodDeleteProcessor.hpp"
#include "MethodGetProcessor.hpp"
#include "MethodPostProcessor.hpp"
#include "WaitProcessor.hpp"

ErrorPageProcessor::ErrorPageProcessor(IClient& client) : client_(client) {}

static std::string intToString(int number) {
  std::stringstream ss;
  ss << number;
  return ss.str();
}

ProcessResult ErrorPageProcessor::process() {
  response_ = client_.getResponse();

  const int code = response_.getStatusCode();
  const std::string msg = response_.getStatusMsg();
  std::stringstream ss;
  ss << "<html>" << CRLF;
  ss << "<head><title>" << code << " " << msg << "</title></head>" << CRLF;
  ss << "<body>" << CRLF;
  ss << "<center><h1>" << code << " " << msg << "</h1></center>" << CRLF;
  ss << "<hr><center>irc</center>" << CRLF;
  ss << "</body>" << CRLF;
  ss << "</html>" << CRLF;
  std::string body = ss.str();
  response_.setHeader("Content-Length", intToString(body.size()));
  response_.setHeader("Connection", "close");
  client_.getDataStream().readStr(response_.toString() + body);
  client_.getDataStream().setEof(true);

  return ProcessResult()
      .setResponse(&response_)
      .setWriteOn(true)
      .setNextProcessor(new WaitProcessor());
}
