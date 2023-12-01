#include "ParseRequestBodyProcessor.hpp"

#include <exception>

#include "ErrorPageProcessor.hpp"
#include "SelectMethodProcessor.hpp"

ParseRequestBodyProcessor::ParseRequestBodyProcessor(IClient& client)
    : readStatus_(HEADER),
      contentLength_(0),
      client_(client),
      request_(client.getRequest()) {}

ProcessResult ParseRequestBodyProcessor::process() {
  try {
    setContentLength();
    if (readStatus_ == BODY && body_.empty()) {
      body_ = client_.getRecvBuffer().nextBuffer(contentLength_);
      if (!body_.empty() || contentLength_ == 0) {
        parseBody();
      }
    }
    if (readStatus_ == DONE) {
      printParseBodyResult();
      client_.setRequest(request_);
      return ProcessResult().setNextProcessor(
          new SelectMethodProcessor(client_));
    }
  } catch (std::exception& e) {
    client_.setResponseStatusCode(402);
    return ProcessResult().setNextProcessor(new ErrorPageProcessor(client_));
  }
  return ProcessResult();
}

void ParseRequestBodyProcessor::setContentLength() {
  std::map<std::string, std::string>::const_iterator it =
      client_.getRequest().getHeaders().find("Content-Length");
  if (it != client_.getRequest().getHeaders().end()) {
    char* end;
    double contentLen = std::strtod(it->second.c_str(), &end);
    if ((end && *end != '\0') || contentLen < 0) {
      throw std::length_error("wrong Content-Length format");  // debug
    }
    contentLength_ = static_cast<size_t>(contentLen);
  }
  readStatus_ = BODY;
}

void ParseRequestBodyProcessor::parseBody() {
  if (body_.size() > contentLength_) {
    throw std::invalid_argument("Body size error");
  }
  request_.setBody(request_.getBody() + body_);
  if (body_.size() == contentLength_) {
    readStatus_ = DONE;
  }
}

void ParseRequestBodyProcessor::printParseBodyResult() {
  std::cout << "========Body=========\n";
  std::cout << body_ << "$" << std::endl;
  std::cout << "=====================\n";
}
