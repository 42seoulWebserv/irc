#include <sys/types.h>
#include <sys/socket.h>
#include <sys/event.h>

#include <iostream>

#include "ServerEventController.hpp"

int run() {
  int kq = kqueue();
  ServerEventController *serverEventController = new ServerEventController(kq);

  while(1) {
    struct kevent eventList[5];
    int number = kevent(kq, 0, 0, eventList, 5, NULL);
    std::cout << "start!!" << std::endl;
    for (int i = 0; i < number; i++) {
      EventController *connector = reinterpret_cast<EventController *>(eventList[i].udata);
      EventController::returnType type = connector->handleEvent(eventList[i]);
      if (type == EventController::SUCCESS || type == EventController::FAIL) {
        delete connector;
      }
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