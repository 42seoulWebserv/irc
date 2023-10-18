#include <iostream>
#include <unistd.h>
#include <sys/event.h>

#include "ClientEventController.hpp"
#include "EventController.hpp"

ClientEventController::ClientEventController(int kq, int clientSocket) : kq_(kq), clientSocket_(clientSocket) {}

ClientEventController::ClientEventController(const ClientEventController &src) : kq_(src.kq_), clientSocket_(src.clientSocket_) {}

ClientEventController &ClientEventController::operator=(const ClientEventController &rhs) {
  this->kq_ = rhs.kq_;
  this->clientSocket_ = rhs.clientSocket_;
  return *this;
}

ClientEventController::~ClientEventController() {}

enum EventController::returnType ClientEventController::handleEvent(const struct kevent &event) {
  if (event.filter == EVFILT_READ) {
    std::cout << "------- client read" << std::endl;
    char buff_rcv[BUFF_SIZE];
    int tmp  = read(event.ident, buff_rcv, BUFF_SIZE);
    if (tmp == 0 || tmp == -1){
      struct kevent clientEvent;

      EV_SET(&clientEvent, event.ident, EVFILT_READ, EV_DELETE, 0, 0, NULL);
      kevent(kq_, &clientEvent, 1, NULL, 0, 0);
    }
    std::cout << "receive: " << buff_rcv << std::endl;
  }
  return OK;
}