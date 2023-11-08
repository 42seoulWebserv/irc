#include "RootConfig.hpp"

RootConfig::RootConfig() : limitClientBodySize_(2) {}

RootConfig::RootConfig(const RootConfig &src) { *this = src; }

RootConfig &RootConfig::operator=(const RootConfig &rhs) {
  if (this == &rhs) {
    return *this;
  }
  this->rootPath_ = rhs.rootPath_;
  this->limitClientBodySize_ = rhs.limitClientBodySize_;
  this->serverConfigs_ = rhs.serverConfigs_;
  return *this;
}

RootConfig::~RootConfig() {}

void RootConfig::printRootConfig() {
  std::vector<ServerConfig>::iterator server;
  std::cout << "root: " << this->rootPath_ << '\n';
  std::cout << "client_max_body_size: " << this->limitClientBodySize_ << '\n';
  for (server = this->serverConfigs_.begin();
       server != this->serverConfigs_.end(); server++) {
    server->printServerConfig();
  }
}

std::string RootConfig::getRootPath() const {
  return rootPath_;
}

void RootConfig::setRootPath(const std::string &rootPath) {
  rootPath_ = rootPath;
}

int RootConfig::getLimitClientBodySize() const {
  return limitClientBodySize_;
}

void RootConfig::setLimitClientBodySize(const int &limitClientBodySize) {
  limitClientBodySize_ = limitClientBodySize;
}

std::vector<ServerConfig> &RootConfig::getServerConfigs() {
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
