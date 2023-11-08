
#include <sys/event.h>
#include <unistd.h>

#include <fstream>
#include <iostream>
#include <sstream>

#include "ClientEventController.hpp"
#include "EventController.hpp"

static bool hasSpace(std::string str) {
  for (size_t i = 0; i < str.length(); i++) {
    if (isspace(str[i])) {
      return true;
    }
  }
  return false;
}

static std::string strTrim(const std::string &str)
{
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

void ClientEventController::printParseResult() {
  std::cout << "=====ParseResult=====\n";
  std::cout << "method: " << method_ << std::endl;
  std::cout << "uri: " << uri_ << std::endl;
  std::cout << "version: " << version_ << std::endl;
  std::cout << headers_ << std::endl;
  std::cout << "======================\n";
}

void ClientEventController::parseHeaderLineByLine(std::string str) {
  size_t i = str.find(':');
  if (i == std::string::npos) {
    throw std::invalid_argument('"' + str + '"' + "no :");
  }
  std::string key = str.substr(0, i);
  if (hasSpace(key)) {
    throw std::invalid_argument('"' + str + '"' + "key has sapce");
  }
  std::string value = strTrim(str.substr(i + 2, std::string::npos));
  headers_.insert(std::make_pair(key, value));
}

void ClientEventController::parseStartLine(std::string str) {
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
  method_ = method;
  size_t j = str.rfind(' ');
  if (j == std::string::npos || j == i) {
    throw std::invalid_argument("no space");
  }
  std::string uri = str.substr(i + 1, j - i);
  uri_ = uri;
  std::string httpVersion = strTrim(str.substr(j + 1, std::string::npos));
  httpVersion = strTrim(httpVersion);
  if (httpVersion != "HTTP/1.1") {
    throw std::invalid_argument("wrong HTTP version");
  }
  version_ = httpVersion;
}

void ClientEventController::parseHeader() {
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

void ClientEventController::parseBody() {
  if (bodyBuffer_.size() > contentLength_) {
    throw std::invalid_argument("Content-Length size error");
  }
}

/* startline, header : 제대로 들어왔는지, 클라이언트가 무엇을 원하는지 확인. */
/* body는 필요한 순간에 읽는다, 여기선 header 내용만 검사한다. */
/* 하나의 요청이 여러 개로 찢어져서 들어온 건지, 다 들어왔는지도 확인. */
/* 주: cheseo 부: yonshin 검: junmkang */
enum EventController::returnType ClientEventController::clientRead(
    const struct kevent &event) {

  if (event.flags & EV_EOF) { // closed socket
    close(clientSocket_);
    return SUCCESS;
  }

  char recvBuff[event.data + 1];
  int tmpInt = read(event.ident, recvBuff, event.data);
  if (tmpInt == -1) {
    evSet(EVFILT_READ, EV_DELETE);
    return FAIL;
  }
  recvBuff[tmpInt] = '\0';
  std::string tmpStr(recvBuff);
  if (readStatus_ != BODY) {
    headerBuffer_ += tmpStr;
    std::cout << headerBuffer_ << std::endl;
    if (headerBuffer_.find("\r\n\r\n") != std::string::npos) {
      readStatus_ = BODY;
      int idx = headerBuffer_.find("\r\n\r\n");
      bodyBuffer_  += headerBuffer_.substr(idx + 4, std::string::npos);
      headerBuffer_.erase(idx, std::string::npos);
      std::cout << headerBuffer_ << "$" << std::endl;
      try {
        parseHeader();
      } catch (std::exception &e) {
        std::cout << "Error: " << e.what() << std::endl;  // debug
        statusCode_ = 401;
        evSet(EVFILT_READ, EV_DELETE);
        evSet(EVFILT_WRITE, EV_ADD);
        return PENDING;
      }
      std::map<std::string, std::string>::iterator it =
          headers_.find("Content-Length");
      if (it != headers_.end()) {
        char *end;
        double contentLen = std::strtod(it->second.c_str(), &end);
        if ((end && *end != '\0') || contentLen < 0) {
          statusCode_ = 402;
          throw std::invalid_argument("wrong Content-Length format");
        }
        contentLength_ = static_cast<size_t>(contentLen);
      }
      parseBody();
      printParseResult();
      evSet(EVFILT_READ, EV_DELETE);
      evSet(EVFILT_WRITE, EV_ADD);
    }
    return PENDING;
  } else {
    try {
      bodyBuffer_ += tmpStr;
      parseBody();
    } catch (std::exception &e) {
      std::cout << "Error: " << e.what() << std::endl;  // debug
      statusCode_ = 401;
      evSet(EVFILT_READ, EV_DELETE);
      evSet(EVFILT_WRITE, EV_ADD);
      return PENDING;
    }
  }
  printParseResult();
  evSet(EVFILT_READ, EV_DELETE);
  evSet(EVFILT_WRITE, EV_ADD);
  return PENDING;
}
