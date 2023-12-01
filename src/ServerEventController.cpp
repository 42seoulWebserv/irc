#include "ServerEventController.hpp"

#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <stdexcept>

#include "AcceptClientProcessor.hpp"
#include "Multiplexer.hpp"

ServerEventController::ServerEventController(int port)
    : EventController(new AcceptClientProcessor(*this)), port_(port) {}
ServerEventController::~ServerEventController() {}
ServerEventController *ServerEventController::addEventController(int port) {
  ServerEventController *server = new ServerEventController(port);
  if (server == NULL) {
    return NULL;
  }
  Multiplexer::getInstance().addReadEvent(server->fd_, server);
  return server;
}

void ServerEventController::addServerConfig(ServerConfig *serverConfig) {
  serverConfigs_.push_back(serverConfig);
}

const std::vector<ServerConfig *> &ServerEventController::getServerConfigs()
    const {
  return serverConfigs_;
}

void ServerEventController::init() {
  fd_ = socket(PF_INET, SOCK_STREAM, 0);
  if (fd_ == -1) {
    throw std::runtime_error("socket error");
  }
  fcntl(fd_, F_SETFL, O_NONBLOCK);
  fcntl(fd_, F_SETFD, FD_CLOEXEC);

  struct sockaddr_in addr;

  std::memset(&addr, 0, sizeof(addr));

  int opt = 1;
  if (setsockopt(fd_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int))) {
    throw std::runtime_error("setsockopt error");
  }

  addr.sin_family = AF_INET;                 // IPv4 인터넷 프로토롤
  addr.sin_port = htons(port_);              // 사용할 port 번호는 port
  addr.sin_addr.s_addr = htonl(INADDR_ANY);  // 32bit IPV4 주소

  if (bind(fd_, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
    throw std::logic_error("bind error");
  }
  if (listen(fd_, 128) == -1) {
    throw std::logic_error("listen error");
  }
  Multiplexer::getInstance().addReadEvent(fd_, this);
}

void ServerEventController::handleEvent(const Multiplexer::Event &event) {
  if (event.filter == WEB_WRITE || event.filter == WEB_TIMEOUT) {
    std::cerr << "invalid server filter" << std::endl;
    return;
  }
  if (loopProcess()) {
    std::cout << "accept error" << std::endl;
  }
}

int ServerEventController::acceptClient() {
  socklen_t client_addr_size;
  sockaddr_in client_addr;
  client_addr_size = sizeof(client_addr);

  return accept(fd_, (struct sockaddr *)&client_addr, &client_addr_size);
}
