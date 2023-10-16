#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>
#include <sys/time.h>
#include <sys/event.h>
#include <sys/types.h>

#include "ConfigChecker.hpp"
#include "ConfigLexer.hpp"
#include "ConfigMaker.hpp"
#include "Directive.hpp"
#include "ConfigLexer.hpp"

#define BUFF_SIZE 1024

int main() {
  int serverSocket;

  serverSocket = socket(PF_INET, SOCK_STREAM, 0);
  if (serverSocket == -1) return 1;

  struct sockaddr_in addr;

  memset(&addr, 0, sizeof(addr));

  addr.sin_family      = AF_INET;              // IPv4 인터넷 프로토롤 
  addr.sin_port        = htons(4000);         // 사용할 port 번호는 4000
  addr.sin_addr.s_addr = htonl(INADDR_ANY);   // 32bit IPV4 주소

  if (bind(serverSocket, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
    return 1;	
  }
  if (listen(serverSocket, 5) == -1) {
    return 1;
  }

  int kq = kqueue();

  struct kevent event;

  EV_SET(&event, serverSocket, EVFILT_READ, EV_ADD, 0, 0, NULL);
  kevent(kq, &event, 1, NULL, 0, 0);


  while(1) {
    socklen_t  client_addr_size;
    sockaddr_in client_addr;
    client_addr_size = sizeof(client_addr); // client 주소의 크기

    struct kevent eventList[5];
    int number = kevent(kq, 0, 0, eventList, 5, NULL);
    std::cout << "start!!" << std::endl;
    for (int i = 0; i < number; i++) {
      std::cout << eventList[i].ident <<std::endl;
      if (eventList[i].ident == (uintptr_t)serverSocket) {
        std::cout << "---------- client accept" << std::endl;
        int client_socket = accept(eventList[i].ident, (struct sockaddr*)&client_addr, &client_addr_size);
        if (client_socket == -1) {
          return 1;
        }
        struct kevent clientEvent;

        EV_SET(&clientEvent, client_socket, EVFILT_READ, EV_ADD, 0, 0, NULL);
        kevent(kq, &clientEvent, 1, NULL, 0, 0);
      }
      else {
        std::cout << "------- client read" << std::endl;
        char buff_rcv[BUFF_SIZE];
        int tmp  = read(eventList[i].ident, buff_rcv, BUFF_SIZE);
        if (tmp == 0 || tmp == -1){
          std::cout << "FUCK YOU" << std::endl;
          struct kevent clientEvent;

          EV_SET(&clientEvent, eventList[i].ident, EVFILT_READ, EV_DELETE, 0, 0, NULL);
          kevent(kq, &clientEvent, 1, NULL, 0, 0);
        }
        std::cout << "receive: " << buff_rcv << std::endl;
      }
    }
    std::cout << "end!!" << std::endl;


    // sprintf(buff_snd, "%d : %s", strlen(buff_rcv), buff_rcv);
    // write(client_socket, "write", strlen(buff_snd) + 1); // +1: NULL까지 포함해서 전송
    // close(client_socket);
  }
}
