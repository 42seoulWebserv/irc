#include <sys/event.h>
#include <unistd.h>

#include <iostream>

#include "ClientEventController.hpp"
#include "EventController.hpp"

/* 클라이언트가 저희가 설정한 시간동안 아무것도 보내지않은 경우 */
enum EventController::returnType ClientEventController::clientTimeout(
    const struct kevent &event) {
  (void)event;
  std::cout << "타이머 이벤트 발생!!" << std::endl;
  close(this->clientSocket_);

  return FAIL;
}
