#include <iostream>
#include <unistd.h>
#include <sys/event.h>

#include "ClientEventController.hpp"
#include "EventController.hpp"

static ServerConfig *selectServerConfig(
    std::map<std::string, std::string> &headers,
    std::vector<ServerConfig *> serverConfigs) {
  if (headers.find("Host") == headers.end()) {
    return NULL;
  }
  std::string host = headers["Host"].substr(0, headers["Host"].find(":"));
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

static std::vector<std::string> strSplit(const std::string &str, char delim)
{
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

static bool isParentPath(const std::string &parent, const std::string &child)
{
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

static std::string strTrim(const std::string &str)
{
  size_t start = 0;
  size_t end = str.size();
  for (size_t i = 0; i < str.size(); i++) {
    if (isspace(str[i])) {
      start++;
    } else {
      break;
    }
  }
  for (size_t i = str.size() - 1; i >= 0; i--) {
    if (isspace(str[i])) {
      end--;
    } else {
      break;
    }
  }
  return str.substr(start, end - start);
}

static const LocationConfig *selectLocationConfig(const std::vector<LocationConfig> &locations, const std::string &uri) {
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

enum EventController::returnType ClientEventController::clientWrite(const struct kevent &event)
{
  if (config_ == NULL) {
    ServerConfig *serverConfig = selectServerConfig(headers_, getServerConfigs());
    if (serverConfig == NULL) {
      statusCode_ = 400;
      return PENDING;
    }
    config_ = selectLocationConfig(serverConfig->getLocationConfigs(), strTrim(uri_));
    if (config_ == NULL) {
      statusCode_ = 404;
      return PENDING;
    }
    std::cout << "selected location path: " << config_->getUri() << std::endl;
  }



  std::cout << "statusCode_ : " << this->statusCode_ << std::endl;
  write(event.ident, "HTTP/1.1 200 OK\r\n", 17);
  write(event.ident, "Host: localhost:420\r\n", 21);
  write(event.ident, "Content-Length: 6\r\n", 19);
  write(event.ident, "Content-Type: text/html\r\n", 25);
  write(event.ident, "\r\n", 2);
  write(event.ident, "hello\n", 6);
  evSet(EVFILT_WRITE, EV_DELETE); // write 이벤트를 안 받는다
  return SUCCESS;
}
