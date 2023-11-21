#include "ParseRequestProcessor.hpp"

#include <iostream>
#include <sstream>
#include <vector>

#include "ErrorPageProcessor.hpp"
#include "SelectMethodProcessor.hpp"
#include "ServerConfig.hpp"

ParseRequestProcessor::ParseRequestProcessor(IClient &client)
    : client_(client) {}

static bool hasSpace(std::string str) {
  for (size_t i = 0; i < str.length(); i++) {
    if (isspace(str[i])) {
      return true;
    }
  }
  return false;
}

static std::string strTrim(const std::string &str) {
  size_t start = 0;
  size_t end = str.size();
  for (size_t i = 0; i < str.size(); i++) {
    if (std::isspace(str[i])) {
      start++;
    } else {
      break;
    }
  }
  for (size_t i = str.size() - 1; i >= 0; i--) {
    if (std::isspace(str[i])) {
      end--;
    } else {
      break;
    }
  }
  return str.substr(start, end - start);
}

void ParseRequestProcessor::printParseResult() {
  std::cout << "=====ParseResult=====\n";
  std::cout << "method: " << request_.getMethod() << std::endl;
  std::cout << "uri: " << request_.getUri() << std::endl;
  std::cout << "version: " << request_.getVersion() << std::endl;
  std::map<std::string, std::string>::const_iterator iter;
  for (iter = request_.getHeaders().begin();
       iter != request_.getHeaders().end(); iter++) {
    std::cout << iter->first << ": " << iter->second << std::endl;
  }
  std::cout << "======================\n";
}

void ParseRequestProcessor::parseHeaderLineByLine(std::string str) {
  size_t i = str.find(':');
  if (i == std::string::npos) {
    throw std::invalid_argument('"' + str + '"' + " no :");
  }
  std::string key = str.substr(0, i);
  if (hasSpace(key)) {
    throw std::invalid_argument('"' + str + '"' + " key has space");
  }
  std::string value = strTrim(str.substr(i + 2, std::string::npos));
  request_.setHeader(key, value);
}

void ParseRequestProcessor::parseStartLine(std::string str) {
  if (*str.end() == ' ') {
    str.pop_back();
  }
  size_t i = str.find(' ');
  if (i == std::string::npos) {
    throw std::invalid_argument("no space");
  }
  std::string method = str.substr(0, i);
  if (method != "GET" && method != "POST" && method != "DELETE") {
    throw std::invalid_argument("wrong method");
  }
  request_.setMethod(method);
  size_t j = str.rfind(' ');
  if (j == std::string::npos || j == i) {
    throw std::invalid_argument("no space");
  }
  std::string uri = str.substr(i + 1, j - i - 1);
  std::cout << "uri: " << uri << std::endl;
  request_.setUri(uri);
  std::string httpVersion = strTrim(str.substr(j + 1, std::string::npos));
  httpVersion = strTrim(httpVersion);
  if (httpVersion != "HTTP/1.1") {
    throw std::invalid_argument("wrong HTTP version");
  }
  request_.setVersion(httpVersion);
}

void ParseRequestProcessor::parseHeader() {
  std::stringstream readBuff(headerBuffer_);
  std::string split;
  bool isStartLine = true;
  while (std::getline(readBuff, split, '\n')) {
    if (isStartLine) {
      readStatus_ = START_LINE;
      parseStartLine(split);
      isStartLine = false;
    } else {
      readStatus_ = HEADER;
      parseHeaderLineByLine(split);
    }
  }
}

void ParseRequestProcessor::parseBody() {
  if (bodyBuffer_.size() > contentLength_) {
    throw std::invalid_argument("Content-Length size error");
  }
  request_.setBody(bodyBuffer_);
}

ProcessResult ParseRequestProcessor::process() {
  std::string tmpStr(client_.getRecvBuffer().begin(),
                     client_.getRecvBuffer().end());
  if (readStatus_ != BODY) {
    headerBuffer_ += tmpStr;
    std::cout << headerBuffer_ << std::endl;  // debug
    if (headerBuffer_.find("\r\n\r\n") != std::string::npos) {
      readStatus_ = BODY;
      int idx = headerBuffer_.find("\r\n\r\n");
      bodyBuffer_ += headerBuffer_.substr(idx + 4, std::string::npos);
      headerBuffer_.erase(idx, std::string::npos);
      std::cout << headerBuffer_ << "$" << std::endl;  // debug
      try {
        parseHeader();
      } catch (std::exception &e) {
        std::cout << "error: " << e.what() << std::endl;  // debug
        return ProcessResult().setStatus(401).setNextProcessor(
            new ErrorPageProcessor(client_));
      }
      std::map<std::string, std::string>::const_iterator it =
          request_.getHeaders().find("Content-Length");
      if (it != request_.getHeaders().end()) {
        char *end;
        double contentLen = std::strtod(it->second.c_str(), &end);
        if ((end && *end != '\0') || contentLen < 0) {
          std::cout << "error: wrong Content-Length format"
                    << std::endl;  // debug
          return ProcessResult().setStatus(402).setNextProcessor(
              new ErrorPageProcessor(client_));
        }
        contentLength_ = static_cast<size_t>(contentLen);
      }
      parseBody();
      printParseResult();  // debug
      return ProcessResult().setRequest(&request_).setNextProcessor(
          new SelectMethodProcessor(client_));
    }
    return ProcessResult();
  } else {
    try {
      bodyBuffer_ += tmpStr;
      parseBody();
    } catch (std::exception &e) {
      std::cout << "Error: " << e.what() << std::endl;  // debug
      return ProcessResult().setStatus(401).setNextProcessor(
          new ErrorPageProcessor(client_));
    }
  }
  printParseResult();  // debug
  return ProcessResult().setRequest(&request_).setNextProcessor(
      new SelectMethodProcessor(client_));
}
