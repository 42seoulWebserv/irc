#include <fcntl.h>
#include <netinet/in.h>
#include <sys/event.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include <iostream>

#include "BodyStream.hpp"
int main() {
  int kq = kqueue();

  int soc = socket(PF_INET, SOCK_STREAM, 0);
  int opt = 1;
  setsockopt(soc, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int));
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(80);
  addr.sin_addr.s_addr = htonl(INADDR_ANY);
  bind(soc, (struct sockaddr *)&addr, sizeof(addr));
  listen(soc, 5);
  sockaddr_in client_addr;
  socklen_t client_addr_len = sizeof(client_addr);
  int clientSocket =
      accept(soc, (struct sockaddr *)&client_addr, &client_addr_len);
  struct kevent event;

  EV_SET(&event, clientSocket, EVFILT_WRITE, EV_ADD, 0, 0, NULL);
  kevent(kq, &event, 1, NULL, 0, 0);

  int filefd = open("tmp", O_RDONLY);
  EV_SET(&event, filefd, EVFILT_READ, EV_ADD, 0, 0, NULL);
  kevent(kq, &event, 1, NULL, 0, 0);
  BodyStream body;

  while (1) {
    struct kevent eventList[5];
    int number = kevent(kq, 0, 0, eventList, 5, NULL);
    for (int i = 0; i < number; i++) {
      if (eventList[i].filter == EVFILT_READ && eventList[i].ident == filefd) {
        int size = body.readFrom(filefd);
        std::cout << "read: " << size << std::endl;
      }
      if (eventList[i].filter == EVFILT_WRITE && body.isEOF() == false) {
        int size = body.writeTo(clientSocket);
        std::cout << "write: " << size << std::endl;
      }
    }
  }
}