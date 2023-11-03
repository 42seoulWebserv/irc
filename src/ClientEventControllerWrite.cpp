#include <iostream>
#include <unistd.h>
#include <sys/event.h>

#include "ClientEventController.hpp"
#include "EventController.hpp"

ServerConfig *ClientEventController::selectServerConfig()
{
  if (headers_.find("Host") == headers_.end()) {
    return NULL;
  }
  std::string host = headers_["Host"].substr(0, headers_["Host"].find(":"));
  std::vector<ServerConfig *> serverConfigs = getServerConfigs();
  if (serverConfigs.size() == 0) {
    return NULL;
  }
  std::vector<ServerConfig *> candidates;
  for (size_t i = 0; i < serverConfigs.size(); i++) {
    if (serverConfigs[i]->getServerName() == host) {
      candidates.push_back(serverConfigs[i]);
    }
  }
  ServerConfig *config = getServerConfigs()[0];
  if (candidates.size() > 0) {
    config = candidates[0];
  }
  return config;
}

enum EventController::returnType ClientEventController::clientWrite(const struct kevent &event)
{
  if (config_ == NULL) {
    config_ = selectServerConfig();
    if (config_ == NULL) {
      statusCode_ = 400;
    }
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
