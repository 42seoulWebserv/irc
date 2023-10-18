#include <sys/types.h>
#include <sys/socket.h>
#include <sys/event.h>

#include <iostream>

#include "ServerConnector.hpp"

int run() {
  int kq = kqueue();
  ServerConnector *serverConnector = new ServerConnector(kq);

  while(1) {
    struct kevent eventList[5];
    int number = kevent(kq, 0, 0, eventList, 5, NULL);
    std::cout << "start!!" << std::endl;
    for (int i = 0; i < number; i++) {
      Connector *connector = reinterpret_cast<Connector *>(eventList[i].udata);
      connector->handleEvent(eventList[i]);
    }
    std::cout << "end!!" << std::endl;

    // sprintf(buff_snd, "%d : %s", strlen(buff_rcv), buff_rcv);
    // write(client_socket, "write", strlen(buff_snd) + 1); // +1: NULL까지 포함해서 전송
    // close(client_socket);
  }
  return 0;
}

int main() {
  run();
  return 0;
}