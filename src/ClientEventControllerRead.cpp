
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

void ClientEventController::printParseResult() {
  std::cout << "=====ParseResult=====\n";
  std::cout << "method: " << request_.getMethod() << std::endl;
  std::cout << "uri: " << request_.getUri() << std::endl;
  std::cout << "version: " << request_.getVersion() << std::endl;
  std::cout << request_.getHeaders() << std::endl;
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
  if (!request_.hasHeader("Content-Type")) {
    request_.setHeader("Content-Type", "text/html");
  }
  request_.setHeader(key, value);
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
  request_.setMethod(method);
  size_t j = str.rfind(' ');
  if (j == std::string::npos || j == i) {
    throw std::invalid_argument("no space");
  }
  std::string uri = str.substr(i + 1, j - i);
  request_.setUri(uri);
  std::string httpVersion = strTrim(str.substr(j + 1, std::string::npos));
  httpVersion = strTrim(httpVersion);
  if (httpVersion != "HTTP/1.1") {
    throw std::invalid_argument("wrong HTTP version");
  }
  request_.setVersion(httpVersion);
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

static ServerConfig *
selectServerConfig(RequestVO request,
                   std::vector<ServerConfig *> serverConfigs) {
  if (request.hasHeader("Host") == false) {
    return NULL;
  }
  std::string host = request.getHeader("Host");
  host = host.substr(0, host.find(":"));
  if (serverConfigs.size() == 0) {
    return NULL;
  }
  std::vector<ServerConfig *> candidates;
  for (size_t i = 0; i < serverConfigs.size(); i++) {
    if (serverConfigs[i]->getServerName() == host) {
      candidates.push_back(serverConfigs[i]);
    }
  }
  ServerConfig *config = serverConfigs[0];
  if (candidates.size() > 0) {
    config = candidates[0];
  }
  return config;
}

static std::vector<std::string> strSplit(const std::string &str, char delim) {
  std::vector<std::string> result;
  size_t prev = 0;
  for (size_t i = 0; i < str.size(); i++) {
    if (str[i] == delim) {
      result.push_back(str.substr(prev, i - prev));
      prev = i + 1;
    }
  }
  if (prev < str.size()) {
    result.push_back(str.substr(prev, str.size() - prev));
  }
  return result;
}

static bool isParentPath(const std::string &parent, const std::string &child) {
  if (parent.size() > child.size()) {
    return false;
  }
  const std::vector<std::string> parentSplit = strSplit(parent, '/');
  const std::vector<std::string> childSplit = strSplit(child, '/');
  if (parentSplit.size() > childSplit.size()) {
    return false;
  }
  for (size_t i = 0; i < parentSplit.size(); i++) {
    if (parentSplit[i] != childSplit[i]) {
      return false;
    }
  }
  return true;
}

static const LocationConfig *
selectLocationConfig(const std::vector<LocationConfig> &locations,
                     const std::string &uri) {
  const LocationConfig *res = NULL;
  size_t maxLocationLen = 0;
  for (size_t i = 0; i < locations.size(); i++) {
    const std::string &locationURI = locations[i].getUri();
    if (isParentPath(locationURI, uri) == false) {
      continue;
    }
    if (maxLocationLen > locationURI.size()) {
      continue;
    }
    if (locationURI[locationURI.size() - 1] != '/' && locationURI != uri) {
      continue;
    }
    maxLocationLen = locationURI.size();

    res = &locations[i];
  }
  return res;
}

void ClientEventController::beginProcess(int statusCode) {
  evSet(EVFILT_READ, EV_DELETE);
  statusCode_ = statusCode;
  if (statusCode >= 200) {
    // TODO: 적절한 processor 처리 필요.
    std::cout << "error code: " << statusCode << std::endl;
    return;
  }
  if (config_ == NULL) {
    ServerConfig *serverConfig =
        selectServerConfig(request_, getServerConfigs());
    if (serverConfig == NULL) {
      statusCode_ = 400;
      // TODO: 적절한 processor 처리 필요.
      return;
    }
    config_ = selectLocationConfig(serverConfig->getLocationConfigs(),
                                   strTrim(request_.getUri()));
    if (config_ == NULL) {
      statusCode_ = 404;
      // TODO: 적절한 processor 처리 필요.
      return;
    }
    std::cout << "debug - selected location path: " << config_->getUri()
              << std::endl;
  }
  processor_ = RequestProcessorFactory::createRequestProcessor(
      request_, config_, kq_, this);
}

/* startline, header : 제대로 들어왔는지, 클라이언트가 무엇을 원하는지 확인. */
/* body는 필요한 순간에 읽는다, 여기선 header 내용만 검사한다. */
/* 하나의 요청이 여러 개로 찢어져서 들어온 건지, 다 들어왔는지도 확인. */
/* 주: cheseo 부: yonshin 검: junmkang */
enum EventController::returnType
ClientEventController::clientRead(const struct kevent &event) {
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
      bodyBuffer_ += headerBuffer_.substr(idx + 4, std::string::npos);
      headerBuffer_.erase(idx, std::string::npos);
      // std::cout << headerBuffer_ << "$" << std::endl;
      try {
        parseHeader();
      } catch (std::exception &e) {
        std::cout << "Error: " << e.what() << std::endl; // debug
        beginProcess(401);
        return PENDING;
      }
      std::map<std::string, std::string>::const_iterator it =
          request_.getHeaders().find("Content-Length");
      if (it != request_.getHeaders().end()) {
        char *end;
        double contentLen = std::strtod(it->second.c_str(), &end);
        if ((end && *end != '\0') || contentLen < 0) {
          statusCode_ = 402;
          // beginProcess(402); throw 가 필요한가?
          throw std::invalid_argument("wrong Content-Length format");
        }
        contentLength_ = static_cast<size_t>(contentLen);
      }
      // bodyBuffer_ += tmpStr;
      parseBody();
      // printParseResult();
      beginProcess(0);
    }
    return PENDING;
  } else {
    try {
      bodyBuffer_ += tmpStr;
      parseBody();
    } catch (std::exception &e) {
      std::cout << "Error: " << e.what() << std::endl; // debug
      beginProcess(401);
      return PENDING;
    }
  }
  printParseResult();
  beginProcess(0);
  return PENDING;
}
