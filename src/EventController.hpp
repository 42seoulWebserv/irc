#ifndef EVENTCONTROLLER_HPP_
#define EVENTCONTROLLER_HPP_

#include "IProcessor.hpp"
#include "Multiplexer.hpp"
#include "ProcessResult.hpp"
#include "RootConfig.hpp"

/* event hadling
** kqueue가 발생시키는 event를 handle
*/
class EventController {
 public:
  EventController(IProcessor *processor);
  virtual ~EventController();

  enum returnType {
    SUCCESS,  // 정상적인 페이지 반환
    FAIL,     // response X, connect 끊기
    ERROR,    // error page 준비
    PENDING,  // 계속 사용
  };

  virtual void init() = 0;
  virtual enum returnType handleEvent(const Multiplexer::Event &event) = 0;

  int getFd() const;

 protected:
  int fd_;
  bool loopProcess();

 private:
  IProcessor *processor_;
  ProcessResult nextProcessor();
};

#endif
