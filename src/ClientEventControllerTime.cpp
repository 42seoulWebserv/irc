#include <iostream>
#include <unistd.h>
#include <sys/event.h>

#include "ClientEventController.hpp"
#include "EventController.hpp"

enum EventController::returnType ClientEventController::clientTimeout(const struct kevent &event)
{
  /* 클라이언트가 저희가 설정한 시간동안 아무것도 보내지않은 경우 */
  /* 주: junmkang 부: cheseo 검: yonshin */
  return TIMEOUT;
}
