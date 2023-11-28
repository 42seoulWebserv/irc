#include "CgiEventController.hpp"

#include <sys/socket.h>
#include <sys/types.h>

#include <stdexcept>

#include "IObserver.hpp"

CgiEventController::CgiEventController() {}

CgiEventController* CgiEventController::addEventController(const Request& req) {
  CgiEventController* cgi = new CgiEventController();
  int fd[2];
  if (socketpair(AF_UNIX, SOCK_STREAM, 0, fd) == -1) {
    delete cgi;
    throw std::runtime_error("socketpair error");
  }
  cgi->fd_ = fd[0];
  // int pid = fork();
  // execve
  Multiplexer::addWriteEvent()
}

CgiEventController* CgiEventController::addEventController(const Request& req) {
  return nullptr;
}

EventController::returnType CgiEventController::handleEvent(
    const Multiplexer::Event& event) {
  if (event.filter == WEB_WRITE) {
    write(pid, request.body(), 100);
  }
  return PENDING;
}

void CgiEventController::appendRequestStr(const std::string& str) {
  data_.readStr(str);
}

void CgiEventController::setCgiObserver(
    IObserver<CgiEventController::Event>* observer) {
  observer_ = observer;
}
