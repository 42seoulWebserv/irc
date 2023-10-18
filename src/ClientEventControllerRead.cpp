#include <iostream>
#include <unistd.h>
#include <sys/event.h>

#include "ClientEventController.hpp"
#include "EventController.hpp"

enum EventController::returnType ClientEventController::clientRead(const struct kevent &event)
{
  /* startline, header : 제대로 들어왔는지, 클라이언트가 무엇을 원하는지 확인 */
  /* body는 필요한 순간에 읽는다, 여기선 header 내용만 검사한다 */
  /* 하나의 요청이 여러 개로 찢어져서 들어온 건지, 다 들어왔는지도 확인*/
  /* 주: cheseo 부: yonshin 검: junmkang */
  std::cout << "------- client read" << std::endl;
  char buff_rcv[BUFF_SIZE];
  int tmp  = read(event.ident, buff_rcv, BUFF_SIZE);
  if (tmp == 0 || tmp == -1){
    struct kevent clientEvent;

    EV_SET(&clientEvent, event.ident, EVFILT_READ, EV_DELETE, 0, 0, NULL);
    kevent(kq_, &clientEvent, 1, NULL, 0, 0);
  }
  /* 검사가 끝나면 wirte 해달라고 켜야함 */
  // if (read all header) {
  //   struct kevent clientEvent;

  //   EV_SET(&clientEvent, event.ident, EVFILT_WRITE, EV_ADD, 0, 0, NULL);
  //   EV_SET(&clientEvent, event.ident, EVFILT_READ, EV_DELETE, 0, 0, NULL);
  //   kevent(kq_, &clientEvent, 1, NULL, 0, 0);
  // }
  std::cout << "receive: " << buff_rcv << std::endl;
  return PENDING;
}