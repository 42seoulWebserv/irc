#ifndef EVENTCONTROLLER_HPP_
#define EVENTCONTROLLER_HPP_

#include "Multiplexer.hpp"
#include "RootConfig.hpp"

/* event hadling
** kqueue가 발생시키는 event를 handle
*/
class EventController {
 public:
  EventController();
  virtual ~EventController();

  enum returnType {
    SUCCESS,  // 정상적인 페이지 반환
    FAIL,     // response X, connect 끊기
    ERROR,    // error page 준비
    PENDING,  // 계속 사용
  };

  virtual enum returnType handleEvent(const Multiplexer::Event &event) = 0;

  void addServerConfig(ServerConfig *serverConfigs);
  void setServerConfigs(const std::vector<ServerConfig *> &serverConfigs);
  const std::vector<ServerConfig *> &getServerConfigs() const;
  int getFd() const;

 protected:
  int fd_;

 private:
  std::vector<ServerConfig *> serverConfigs_;
};

#endif
