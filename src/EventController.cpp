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

std::vector<ServerConfig *> EventController::getServerConfigs() const {
  return serverConfigs_;
}
