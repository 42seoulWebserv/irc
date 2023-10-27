#include <iostream>
#include <sstream>
#include <fstream>

#include <sys/event.h>
#include <unistd.h>

#include "ClientEventController.hpp"
#include "EventController.hpp"

// ": "
static bool hasSpace(std::string str) {
  for (size_t i = 0; i < str.length(); i++)
  {
    if (isspace(str[i])) {
      return true;
    }
  }
  return false;
}

void ClientEventController::printParseResult() {
  std::cout << "=====ParseResult=====\n";
  std::cout << "method: " << method_ << std::endl;
  std::cout << "uri: " << uri_ << std::endl;
  std::cout << "version: " << version_ << std::endl;
  std::cout << headers_ << std::endl;
  std::cout << "=====ParseResult=====\n";
}

void ClientEventController::parseHeaderLineByLine(std::string str) {
  size_t i = str.find(':');
  if (i == std::string::npos) {
    throw std::invalid_argument("no :");
  }
  std::string key = str.substr(0, i);
  if (hasSpace(key)) {
    throw std::invalid_argument("key has sapce");
  }
  std::string value = str.substr(i + 1, str.length());
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
  std::string uri = str.substr(i, j - i);
  uri_ = uri;
  std::string httpVersion = str.substr(j + 1, str.length());
  std::stringstream ss(httpVersion);
  ss >> httpVersion;
  if (httpVersion != "HTTP/1.1") {
    throw std::invalid_argument("wrong HTTP version");
  }
  version_ = httpVersion;
}

/* startline, header : 제대로 들어왔는지, 클라이언트가 무엇을 원하는지 확인. */
/* body는 필요한 순간에 읽는다, 여기선 header 내용만 검사한다. */
/* 하나의 요청이 여러 개로 찢어져서 들어온 건지, 다 들어왔는지도 확인. */
/* 주: cheseo 부: yonshin 검: junmkang */
enum EventController::returnType ClientEventController::clientRead(const struct kevent &event) {
  std::cout << "------- client read" << std::endl;
  char recvBuff[BUFF_SIZE];

  int tmpInt  = read(event.ident, recvBuff, BUFF_SIZE); // 3000 1024 / 1024 / 나머지
  if (tmpInt == -1) {
    isValidReq_ = false;
    struct kevent clientEvent;
    EV_SET(&clientEvent, event.ident, EVFILT_READ, EV_DELETE, 0, 0, NULL);
    EV_SET(&clientEvent, event.ident, EVFILT_WRITE, EV_ADD, 0, 0, NULL);
    kevent(kq_, &clientEvent, 1, NULL, 0, 0);
    return PENDING;
  }
  std::string tmpStr(recvBuff);
  if (status_ != BODY && tmpStr.find('\r\n') == std::string::npos) {
    headerBuffer_ += tmpStr;
    return PENDING;
  }
  if (tmpInt != 0) {
    bodyBuffer_ += tmpStr;
    return PENDING;
  }
  std::stringstream readBuff(headerBuffer_);
  std::string split;
  int idx = 0;
  try {
    while (status_ != BODY && std::getline(readBuff, split, '\n')) {
      if (idx == 0) {
        status_ = START_LINE;
        parseStartLine(split);
      } else {
        status_ = HEADER;
        parseHeaderLineByLine(split);
      }
      idx++;
    }
    if (status_ == BODY) {
      std::map<std::string, std::string>::iterator it = headers_.find("content_length");
      char *end;
      if (it != headers_.end()) {
        double contentLen = std::strtod(it->second.c_str(), &end);
        if (end && *end != '\0') {
          return PENDING;
        }
        if (bodyBuffer_.size() == contentLen) {
          return PENDING;
        }
      }
    }
  } catch (std::exception& e) {
    std::cout << "Error: " << e.what() << std::endl;
    return FAIL;
  }
  printParseResult();
  return PENDING;
  // if (tmp == 0 || tmp == -1){
  //   struct kevent clientEvent;

  //   EV_SET(&clientEvent, event.ident, EVFILT_READ, EV_DELETE, 0, 0, NULL);
  //   kevent(kq_, &clientEvent, 1, NULL, 0, 0);
  // }
  /* 검사가 끝나면 wirte 해달라고 켜야함 */
  // if (read all header) {
  //   struct kevent clientEvent;

  //   EV_SET(&clientEvent, event.ident, EVFILT_WRITE, EV_ADD, 0, 0, NULL);
  //   EV_SET(&clientEvent, event.ident, EVFILT_READ, EV_DELETE, 0, 0, NULL);
  //   kevent(kq_, &clientEvent, 1, NULL, 0, 0);
  // }
  // std::cout << "receive: " << recvBuff << std::endl;
}