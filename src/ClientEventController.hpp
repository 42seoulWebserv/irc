#ifndef CLIENTEVENTCONTROLLER_HPP_
#define CLIENTEVENTCONTROLLER_HPP_

#include <map>
#include <ostream>
#include <string>

#include "EventController.hpp"
#include "IObserver.hpp"
#include "IRequestProcessor.hpp"
#include "KqueueMultiplexer.hpp"
#include "RequestProcessorDispatcher.hpp"
#include "Request.hpp"
#include "ResponseStream.hpp"
#include "Response.hpp"

#define BUFF_SIZE 4
/* server(port) 또는 client가 보낸 요청을 수행하는 클래스 */
class ClientEventController : public EventController,
                              public IObserver<Response> {
 public:
  enum READ_STATUS { START_LINE, HEADER, BODY };

  virtual ~ClientEventController();
  static void addEventController(int socket,
                                 const std::vector<ServerConfig *> &configs);

  enum EventController::returnType handleEvent(const struct kevent &event);

  void onEvent(const Response &p);

 private:
  enum READ_STATUS readStatus_;
  int statusCode_;
  int clientSocket_;
  size_t contentLength_;
  std::string headerBuffer_;
  std::string bodyBuffer_;
  const LocationConfig *config_;

  Request request_;
  Response response_;
  ResponseStream stream_;

  IRequestProcessor *processor_;

  enum EventController::returnType clientRead(const struct kevent &event);
  enum EventController::returnType clientWrite(const struct kevent &event);
  enum EventController::returnType clientTimeout(const struct kevent &event);

  ClientEventController(int clientSocket);
  ClientEventController(const ClientEventController &src);
  ClientEventController &operator=(const ClientEventController &rhs);
  void parseHeaderLineByLine(std::string str);
  void parseStartLine(std::string str);
  void printParseResult();
  void parseHeader();
  void parseBody();
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
