#include "CgiEventController.hpp"

#include <signal.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <map>
#include <sstream>
#include <stdexcept>

#include "CgiInProcessor.hpp"
#include "IObserver.hpp"
#include "Multiplexer.hpp"

CgiEventController::CgiEventController(
    IClient& client, IObserver<CgiEventController::Event>* observer)
    : EventController(new CgiInProcessor(*this)),
      client_(client),
      cancel_(false),
      pid_(0),
      observer_(observer) {}

CgiEventController::~CgiEventController() {
  if (pid_ > 0) {
    std::cout << "kill: " << pid_ << std::endl;
    kill(pid_, SIGKILL);
  }
  close(fd_);
}

void CgiEventController::init() {
  int fd[2];
  socketpair(AF_UNIX, SOCK_STREAM, 0, fd);
  int pid_ = fork();
  if (pid_ == -1) {
    std::cerr << "debug fork error" << std::endl;
    throw std::runtime_error("fork error");
  }
  if (pid_ == 0) {
    close(fd[0]);
    execve("/bin/echo", NULL, NULL);
    exit(1);
  }
  close(fd[1]);
  fd_ = fd[0];
  Multiplexer::getInstance().addWriteEvent(fd_, this);
}

CgiEventController* CgiEventController::addEventController(
    IClient& client, IObserver<CgiEventController::Event>* observer) {
  CgiEventController* cgi = new CgiEventController(client, observer);
  if (cgi == NULL) {
    return NULL;
  }
  try {
    cgi->init();
  } catch (...) {
    delete cgi;
    return NULL;
  }
  return cgi;
}

void CgiEventController::handleEvent(const Multiplexer::Event& event) {
  if (cancel_) {
    cancel();
    return;
  }
  if (event.filter == WEB_WRITE) {
  }
  if (event.filter == WEB_READ) {
  }
  if (loopProcess()) {
    if (observer_) {
      observer_->onEvent(Event());
    }
    Multiplexer::getInstance().addDeleteController(this);
    return;
  }
      if (observer_) {
      observer_->onEvent(Event());
    }
  Multiplexer::getInstance().addDeleteController(this);
}

void CgiEventController::spendBuffer(int size) { (void)size; }

void CgiEventController::cancel() {
  cancel_ = true;
  Multiplexer::getInstance().addDeleteController(this);
}
