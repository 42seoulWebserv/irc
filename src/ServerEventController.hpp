#ifndef SERVEREVENTCONTROLLER_HPP_
#define SERVEREVENTCONTROLLER_HPP_

#include "ClientEventController.hpp"
#include "EventController.hpp"
#include "IServer.hpp"

class ServerEventController : public EventController, public IServer {
 public:
  ~ServerEventController();
  static ServerEventController *addEventController(int port);

  void addServerConfig(ServerConfig *serverConfigs);

  void init();
  enum EventController::returnType handleEvent(const Multiplexer::Event &event);
  void spendBuffer(int size);

  int acceptClient();
  const std::vector<ServerConfig *> &getServerConfigs() const;

 private:
  int port_;
  std::vector<ServerConfig *> serverConfigs_;
  ServerEventController(int port);
};

#endif
