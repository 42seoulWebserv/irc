#ifndef CLIENTEVENTCONTROLLER_HPP_
#define CLIENTEVENTCONTROLLER_HPP_

#include "DataStream.hpp"
#include "EventController.hpp"
#include "Multiplexer.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "ServerConfig.hpp"
#include "StringBuffer.hpp"

#define BUFF_SIZE 4
/* server(port) 또는 client가 보낸 요청을 수행하는 클래스 */
class ClientEventController : public EventController, public IClient {
 public:
  ~ClientEventController();
  static ClientEventController *addEventController(
      int socket, const std::vector<ServerConfig *> &configs);

  void init();
  void handleEvent(const Multiplexer::Event &event);

  const Request &getRequest() const;
  void setRequest(const Request &request);
  const Response &getResponse() const;
  void setResponse(const Response &response);
  void setResponseStatusCode(int code);
  void setResponseHeader(const std::string &key, const std::string &value);
  void setBody(const std::string &body);
  std::string &getBody();
  DataStream &getDataStream();
  StringBuffer &getRecvBuffer();
  FilePath getRequestResourcePath();
  const LocationConfig *getLocationConfig();
  void print(Log &logger, const std::string &msg);

 private:
  const LocationConfig *config_;
  std::vector<ServerConfig *> serverConfigs_;

  StringBuffer buffer_;
  Request request_;
  Response response_;
  DataStream stream_;
  std::string body_;

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
*/ // debug

std::ostream &operator<<(std::ostream &o,
                         const std::map<std::string, std::string> &rhs);

#endif