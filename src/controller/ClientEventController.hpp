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
  DataStream &getResponseStream();
  StringBuffer &getRecvBuffer();
  FilePath getRequestResourcePath();
  const LocationConfig *getLocationConfig();
  void print(Log &logger, const std::string &msg);

 private:
  const LocationConfig *config_;
  std::vector<ServerConfig *> serverConfigs_;

  Request request_;
  Response response_;
  std::string body_;
  StringBuffer recvBuffer_;
  DataStream responseStream_;

  ClientEventController(int clientSocket);
  ClientEventController(const ClientEventController &src);
  ClientEventController &operator=(const ClientEventController &rhs);
  void clearKeepAlive();
  void clearForce();
};

std::ostream &operator<<(std::ostream &o,
                         const std::map<std::string, std::string> &rhs);

#endif
