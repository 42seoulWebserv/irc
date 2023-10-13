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
