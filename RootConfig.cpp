#include "RootConfig.hpp"

RootConfig::RootConfig(void) {}

// RootConfig::RootConfig(Directive Directive) {}

RootConfig::RootConfig(const RootConfig &src) { *this = src; }

RootConfig &RootConfig::operator=(const RootConfig &rhs) {
  if (this == &rhs) return *this;
  this->serverConfigs_ = rhs.serverConfigs_;
  return *this;
}

RootConfig::~RootConfig(void) {}

void RootConfig::FillInConfig(void) {}

void RootConfig::printRootConfig(void) {
  std::vector<ServerConfig>::iterator server;
  for (server = this->serverConfigs_.begin();
       server != this->serverConfigs_.end(); server++) {
    server->printServerConfig();
  }
}
