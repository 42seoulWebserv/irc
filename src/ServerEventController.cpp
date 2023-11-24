#include "ServerEventController.hpp"

#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <stdexcept>

#include "Multiplexer.hpp"

ServerEventController::ServerEventController(int port) : port_(port) {
  this->socket_ = socket(PF_INET, SOCK_STREAM, 0);
  if (this->socket_ == -1) {
    throw std::logic_error("bind error");
  }

  struct sockaddr_in addr;

  std::memset(&addr, 0, sizeof(addr));

  int opt = 1;
  if (setsockopt(this->socket_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int))) {
    throw std::logic_error("bind error");
  }

  addr.sin_family = AF_INET;                 // IPv4 인터넷 프로토롤
  addr.sin_port = htons(port_);              // 사용할 port 번호는 port
  addr.sin_addr.s_addr = htonl(INADDR_ANY);  // 32bit IPV4 주소

  if (bind(this->socket_, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
    throw std::logic_error("bind error");
  }
  if (listen(this->socket_, 128) == -1) {
    throw std::logic_error("bind error");
  }
  Multiplexer::getInstance().addReadEvent(socket_, this);
}

ServerEventController::~ServerEventController() {}

enum EventController::returnType ServerEventController::handleEvent(
    const Multiplexer::Event &event) {
  socklen_t client_addr_size;
  sockaddr_in client_addr;
  client_addr_size = sizeof(client_addr);  // client 주소의 크기

  std::cout << "---------- client accept" << std::endl;
  int clientSocket =
      accept(event.ident, (struct sockaddr *)&client_addr, &client_addr_size);
  if (clientSocket == -1) {
    std::cout << "accept error" << std::endl;
    return PENDING;
  }
  ClientEventController::addEventController(clientSocket, getServerConfigs());
  return PENDING;
}
