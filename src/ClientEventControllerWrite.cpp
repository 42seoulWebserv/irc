#include <iostream>
#include <unistd.h>
#include <sys/event.h>

#include "ClientEventController.hpp"
#include "EventController.hpp"

enum EventController::returnType ClientEventController::clientWrite(const struct kevent &event)
{
  /* 클라이언트가 요청사항으로 보낸것의 config를 확인하여 답장을 보낼 수 있는지, 어떤 것을 보내야하는지, ( 404, 502, 505, ... ) */
  /* 일단 cgi는 제외 */
  /* 주: yonshin 부: junmkang 검: cheseo */
  return PENDING;
}
