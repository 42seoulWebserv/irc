#include "CgiEventController.hpp"

#include <signal.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>

#include <cstring>
#include <sstream>

#include "CgiInProcessor.hpp"
#include "Log.hpp"
#include "Multiplexer.hpp"

CgiEventController::CgiEventController(
    IClient& client, IObserver<CgiEventController::Event>* observer)
    : EventController(new CgiInProcessor(*this, client)),
      client_(client),
      cancel_(false),
      pid_(0),
      observer_(observer) {}

CgiEventController::~CgiEventController() {
  int err;
  if (waitpid(pid_, &err, WNOHANG) == 0) {
    kill(pid_, SIGKILL);
    waitpid(pid_, &err, WNOHANG);
  }
  close(fd_);
}

static std::string cgiPath(IClient& client_) {
  FilePath extension = FilePath::getExtension(client_.getRequest().getUri());
  if (client_.getLocationConfig()->hasCgiProgram('.' + extension)) {
    return client_.getLocationConfig()->getCgiProgram('.' + extension);
  }
  return NULL;
}

void CgiEventController::init() {
  int fd[2];

  socketpair(AF_UNIX, SOCK_STREAM, 0, fd);
  pid_ = fork();
  if (pid_ == -1) {
    Log::error << "fork error" << NL;
    throw std::runtime_error("fork error");
  }

  if (pid_ == 0) {
    close(fd[0]);
    dup2(fd[1], STDIN_FILENO);
    dup2(fd[1], STDOUT_FILENO);
    close(fd[1]);

    std::string root = cgiPath(client_);

    char* program = new char[root.size() + 1];
    std::strcpy(program, root.c_str());

    char* const argv[] = {program, NULL};
    std::vector<char*> envp_vec;
    envp_vec.push_back(
        strdup(("REQUEST_METHOD=" + client_.getRequest().getMethod()).c_str()));

    envp_vec.push_back(strdup(
        ("SERVER_PROTOCOL=" + client_.getRequest().getVersion()).c_str()));
    envp_vec.push_back(
        strdup(("PATH_INFO=" + client_.getRequest().getUri()).c_str()));
    std::stringstream ss;
    ss << client_.getRequest().getBody().size();
    envp_vec.push_back(strdup(("CONTENT_LENGTH=" + ss.str()).c_str()));
    envp_vec.push_back(NULL);
    char* const* envp = &envp_vec[0];
    execve(program, argv, envp);
    perror("execve");
    _exit(1);
  }
  close(fd[1]);
  setFd(fd[0]);
  Multiplexer::getInstance().addReadEvent(fd_, this);
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
    if (observer) {
      observer->onEvent(Event().setError(true));
    }
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
      clear(true);
      return;
    }
    if (size == 0) {
      client_.setBody(recvBuffer_.getBuffer());
      clear(false);
      return;
    }
    buff.resize(size);
    recvBuffer_.addBuffer(buff);
  }
  if (event.filter == WEB_WRITE) {
    int size = writeBuffer_.writeToClient(fd_);
    if (size == -1) {
      clear(true);
      return;
    }
  }
  if (loopProcess()) {
    clear(true);
  }
}

void CgiEventController::cancel() {
  cancel_ = true;
  Multiplexer::getInstance().addDeleteController(this);
}

void CgiEventController::setFd(int& fd) { fd_ = fd; }

int CgiEventController::getFd() { return fd_; }

StringBuffer& CgiEventController::getRecvBuffer() { return recvBuffer_; }

DataStream& CgiEventController::getWriteBuffer() { return writeBuffer_; }

void CgiEventController::clear(bool error) {
  if (client_.getResponse().getStatusCode() == 100) {
    error = true;
  }
  if (observer_) {
    observer_->onEvent(Event().setError(error));
  }
  Multiplexer::getInstance().addDeleteController(this);
}

void CgiEventController::print(Log& logger, const std::string& msg) {
  logger << fd_ << ": " << msg << NL;
}

CgiEventController::Event& CgiEventController::Event::setError(bool error) {
  error_ = error;
  return *this;
}
