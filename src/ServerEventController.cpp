#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/event.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <stdexcept>
#include <cstdlib>
#include <iostream>

#include "ServerEventController.hpp"

ServerEventController::ServerEventController(int kq, int port) : kq_(kq), port_(port) {
  this->socket_ = socket(PF_INET, SOCK_STREAM, 0);
  if (this->socket_ == -1) {
    throw std::logic_error("bind error");
  }

  struct sockaddr_in addr;

  memset(&addr, 0, sizeof(addr));

	int opt = 1;
	if (setsockopt(this->socket_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int))) {
		throw std::logic_error("bind error");
	}

  addr.sin_family = AF_INET;        // IPv4 인터넷 프로토롤
  addr.sin_port = htons(port);     // 사용할 port 번호는 port
  addr.sin_addr.s_addr = htonl(INADDR_ANY);   // 32bit IPV4 주소

  if (bind(this->socket_, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
    throw std::logic_error("bind error");
  }
  if (listen(this->socket_, 5) == -1) {
    throw std::logic_error("bind error");
  }

  struct kevent event;

  EV_SET(&event, this->socket_, EVFILT_READ, EV_ADD, 0, 0, this);
  kevent(kq, &event, 1, NULL, 0, 0);
}

ServerEventController::ServerEventController(const ServerEventController &src) {}

ServerEventController &ServerEventController::operator=(const ServerEventController &rhs) {
  return *this;
}

ServerEventController::~ServerEventController() {}

enum EventController::returnType ServerEventController::handleEvent(const struct kevent &event) {
  socklen_t client_addr_size;
  sockaddr_in client_addr;
  client_addr_size = sizeof(client_addr); // client 주소의 크기

  std::cout << "---------- client accept" << std::endl;
  int clientSocket = accept(event.ident, (struct sockaddr*)&client_addr, &client_addr_size);
  if (clientSocket == -1) {
    return FAIL;
  }
  struct kevent clientEvent;
  struct timespec timeout = {10, 0}; // 10 seconds

  ClientEventController *clientEventController = new ClientEventController(this->kq_, clientSocket);
  clientEventController->setServerConfigs(this->getServerConfigs());
  EV_SET(&clientEvent, clientSocket, EVFILT_READ, EV_ADD, 0, 0, clientEventController);
  kevent(this->kq_, &clientEvent, 1, NULL, 0, &timeout);
  return PENDING;
}
