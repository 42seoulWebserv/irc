#ifndef CLIENTEVENTCONTROLLER_HPP_
#define CLIENTEVENTCONTROLLER_HPP_

#include <map>
#include <ostream>
#include <string>

#include "DataStream.hpp"
#include "EventController.hpp"
#include "IObserver.hpp"
#include "IRequestProcessor.hpp"
#include "Multiplexer.hpp"
#include "Request.hpp"
#include "Response.hpp"

#define BUFF_SIZE 4
/* server(port) 또는 client가 보낸 요청을 수행하는 클래스 */
class ClientEventController : public EventController, public IClient {
 public:
  virtual ~ClientEventController();
  static void addEventController(int socket,
                                 const std::vector<ServerConfig *> &configs);

  enum EventController::returnType handleEvent(const Multiplexer::Event &event);

  const Request &getRequest() const;
  void setRequest(const Request &reqeust);
  const Response &getResponse() const;
  void setResponse(const Response &response);
  void setResponseStatusCode(int code);
  void setResponseHeader(const std::string &key, const std::string &value);
  DataStream &getDataStream();
  const std::vector<char> &getRecvBuffer() const;
  const LocationConfig *getLocationConfig();
  ProcessResult nextProcessor();

 private:
  int clientSocket_;
  const LocationConfig *config_;

  std::vector<char> recvBuffer_;
  Request request_;
  Response response_;
  DataStream stream_;

  IRequestProcessor *processor_;

  ClientEventController(int clientSocket);
  ClientEventController(const ClientEventController &src);
  ClientEventController &operator=(const ClientEventController &rhs);
  void clear(bool forceClose);
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
