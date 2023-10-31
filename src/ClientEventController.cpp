#include <iostream>
#include <unistd.h>
#include <sys/event.h>

#include "ClientEventController.hpp"
#include "EventController.hpp"

ClientEventController::ClientEventController(int kq, int clientSocket) :
  kq_(kq),
  clientSocket_(clientSocket),
  readStatus_(START_LINE)
  {}

ClientEventController::ClientEventController(const ClientEventController &src) :
  kq_(src.kq_),
  clientSocket_(src.clientSocket_),
  readStatus_(src.readStatus_)
  {}

ClientEventController &ClientEventController::operator=(const ClientEventController &rhs) {
  this->kq_ = rhs.kq_;
  this->clientSocket_ = rhs.clientSocket_;
  return *this;
}

ClientEventController::~ClientEventController() {}

enum EventController::returnType ClientEventController::handleEvent(const struct kevent &event) {
  if (event.filter == EVFILT_READ) {
    return clientRead(event);
  }
  else if (event.filter == EVFILT_WRITE) { // cgi
    return clientWrite(event);
  }
  else if (event.filter == EVFILT_TIMER) {
    return clientTimeout(event);
  }
  else {
    return FAIL;
  }
  return SUCCESS;
}

std::ostream& operator<<(std::ostream& o, const std::map<std::string, std::string>& rhs)
{
	for (std::map<std::string, std::string>::const_iterator it = rhs.begin(); it != rhs.end(); it++) {
		o << "key: " << it->first << ", value: " << it->second << '\n';
	}
	return o;
}
