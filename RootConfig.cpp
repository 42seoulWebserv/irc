#include "RootConfig.hpp"

RootConfig::RootConfig() {}

// RootConfig::RootConfig(Directive Directive) {}

RootConfig::RootConfig(const RootConfig &src) { *this = src; }

RootConfig &RootConfig::operator=(const RootConfig &rhs) {
  if (this == &rhs)
    return *this;
  this->serverConfigs_ = rhs.serverConfigs_;
  return *this;
}

RootConfig::~RootConfig() {}

void RootConfig::printRootConfig() {
  std::vector<ServerConfig>::iterator server;
  for (server = this->serverConfigs_.begin();
       server != this->serverConfigs_.end(); server++) {
    server->printServerConfig();
  }
}

std::vector<ServerConfig> RootConfig::getServerConfigs() const {
  return serverConfigs_;
}

void RootConfig::setServerConfigs(const std::vector<ServerConfig> &serverConfigs) {
  serverConfigs_ = serverConfigs;
}

void RootConfig::addServerConfigs(const ServerConfig &serverConfigs) {
  serverConfigs_.push_back(serverConfigs);
}

const std::vector<ServerConfig>::iterator RootConfig::beginServerConfigs() {
  return serverConfigs_.begin();
}

const std::vector<ServerConfig>::iterator RootConfig::endServerConfigs() {
  return serverConfigs_.end();
}
