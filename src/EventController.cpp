#include "EventController.hpp"

EventController::EventController() {}

EventController::~EventController() {}

void EventController::addServerConfig(ServerConfig *serverConfig) {
  serverConfigs_.push_back(serverConfig);
}

void EventController::setServerConfigs(
    const std::vector<ServerConfig *> &serverConfigs) {
  serverConfigs_ = serverConfigs;
}

const std::vector<ServerConfig *> &EventController::getServerConfigs() const {
  return serverConfigs_;
}

int EventController::getFd() const { return fd_; }
