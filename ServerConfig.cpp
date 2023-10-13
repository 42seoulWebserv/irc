#include "ServerConfig.hpp"

ServerConfig::ServerConfig(void) {}

// ServerConfig::ServerConfig(Directive Directive) {}

ServerConfig::ServerConfig(const ServerConfig &src) { *this = src; }

ServerConfig &ServerConfig::operator=(const ServerConfig &rhs) {
  if (this == &rhs) return *this;
  this->port_ = rhs.port_;
  this->limitClientBodySize_ = rhs.limitClientBodySize_;
  this->rootPath_ = rhs.rootPath_;
  this->serverName_ = rhs.serverName_;
  this->errorPages_ = rhs.errorPages_;
  this->locationConfigs_ = rhs.locationConfigs_;
  return *this;
}

ServerConfig::~ServerConfig(void) {}

void ServerConfig::printServerConfig(void) {
  std::cout << "listen: " << this->port_ << '\n';
  std::vector<LocationConfig>::iterator location;
  for (location = this->locationConfigs_.begin();
       location != this->locationConfigs_.end(); location++) {
    location->printLocationConfig();
  }
}
