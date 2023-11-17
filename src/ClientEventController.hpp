#ifndef CLIENTEVENTCONTROLLER_HPP_
#define CLIENTEVENTCONTROLLER_HPP_

#include <map>
#include <ostream>
#include <string>

#include "EventController.hpp"
#include "IObserver.hpp"
#include "IRequestProcessor.hpp"
#include "RequestProcessorFactory.hpp"
#include "RequestVO.hpp"
#include "ResponseStream.hpp"
#include "ResponseVO.hpp"

#define BUFF_SIZE 4
/* server(port) 또는 client가 보낸 요청을 수행하는 클래스 */
class ClientEventController : public EventController,
                              public IObserver<ResponseVO> {
 public:
  enum READ_STATUS { START_LINE, HEADER, BODY };

  ClientEventController(const ClientEventController &src);
  ClientEventController &operator=(const ClientEventController &rhs);
  virtual ~ClientEventController();
  static void addEventController(int kq, int socket,
                                 const std::vector<ServerConfig *> &configs);

  enum EventController::returnType handleEvent(const struct kevent &event);

  void onEvent(const ResponseVO &p);

 private:
  enum READ_STATUS readStatus_;
  int kq_;
  int statusCode_;
  int clientSocket_;
  size_t contentLength_;
  std::string headerBuffer_;
  std::string bodyBuffer_;
  const LocationConfig *config_;

  RequestVO request_;
  ResponseVO response_;
  ResponseStream stream_;

  IRequestProcessor *processor_;

  enum EventController::returnType clientRead(const struct kevent &event);
  enum EventController::returnType clientWrite(const struct kevent &event);
  enum EventController::returnType clientTimeout(const struct kevent &event);

  ClientEventController(int kq, int clientSocket);
  void parseHeaderLineByLine(std::string str);
  void parseStartLine(std::string str);
  void printParseResult();
  void parseHeader();
  void parseBody();
  void evSet(int filter, int action);
  void beginProcess(int statusCode);
};
/*
// 우선 key에 공백 있으면 안됨.
 TEST: 123 // 맨앞 공백
TEST :123 // key에 :가 붙어있지않음
:TEST: 123 // 맨앞에 :
TE ST: 123 // key 사이에 공백
*/

std::ostream &operator<<(std::ostream &o,
                         const std::map<std::string, std::string> &rhs);

#endif
