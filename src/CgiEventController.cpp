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
    : EventController(new CgiInProcessor(*this, client)),
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
  pid_ = fork();
  if (pid_ == -1) {
    std::cerr << "debug fork error" << std::endl;
    throw std::runtime_error("fork error");
  }
  if (pid_ == 0) {
    close(fd[0]);
    dup2(fd[1], STDIN_FILENO);
    dup2(fd[1], STDOUT_FILENO);
    close(fd[1]);

    char* program = "cgi_tester";
    char* argv[] = {program, NULL};
    char* envp[] = {"REQUEST_METHOD=POST", "SERVER_PROTOCOL=HTTP/1.1",
                    "PATH_INFO=/", "CONTENT_LENGTH=14", NULL};
    execve(program, argv, envp);
    perror("execve");
    _exit(1);
  }
  close(fd[1]);
  setFd(fd[0]);
  // // cgiInProcessor part
  // char* request = "get / http/1.1";  // cgiInProcessor에서 보내줄 내용들
  // int requestSize = write(fd_, request, std::strlen(request));
  // perror("write");
  // std::cerr << "parent write size: " << requestSize << std::endl;

  // // cgiOutProcessor part
  // char buffer[9999];
  // int size = read(fd_, buffer, 9999);
  // buffer[size] = 0;
  // std::cerr << "parent read size: " << size << std::endl;
  // std::cerr << "parent read: " << buffer << std::endl;

  Multiplexer::getInstance().addWriteEvent(fd_, this);
  if (loopProcess()) {
    throw std::runtime_error("process error");
  }
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
  if (event.filter == WEB_READ) {
    std::vector<char> buff;
    buff.resize(CGI_RECV_BUFFER);
    int size = recv(fd_, buff.data(), CGI_RECV_BUFFER, 0);
    if (size == -1) {
      if (observer_) {
        observer_->onEvent(Event().setError(true));
      }
      Multiplexer::getInstance().addDeleteController(this);
      return;
    }
    if (size == 0) {
      client_.setBody(recvBuffer_.getBuffer());
      if (observer_) {
        observer_->onEvent(Event());
      }
      Multiplexer::getInstance().addDeleteController(this);
    }
    buff[size] = '\0';
    recvBuffer_.addBuffer(buff);
  }
  if (event.filter == WEB_WRITE) {
    //
  }
  if (loopProcess()) {
    if (observer_) {
      observer_->onEvent(Event().setError(true));
    }
    Multiplexer::getInstance().addDeleteController(this);
  }
}

void CgiEventController::cancel() {
  cancel_ = true;
  Multiplexer::getInstance().addDeleteController(this);
}

void CgiEventController::setFd(int& fd) { fd_ = fd; }

int CgiEventController::getFd() { return fd_; }

StringBuffer& CgiEventController::getRecvBuffer() { return recvBuffer_; }

CgiEventController::Event& CgiEventController::Event::setError(bool error) {
  error_ = error;
  return *this;
}
