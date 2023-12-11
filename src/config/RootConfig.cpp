#include "RootConfig.hpp"

#include <sstream>

#include "Log.hpp"

RootConfig::RootConfig()
    : limitClientBodySize_(1 * 1024 * 1024), autoindex_(false) {}

RootConfig::RootConfig(const RootConfig &src) { *this = src; }

RootConfig &RootConfig::operator=(const RootConfig &rhs) {
  if (this == &rhs) {
    return *this;
  }
  this->rootPath_ = rhs.rootPath_;
  this->limitClientBodySize_ = rhs.limitClientBodySize_;
  this->autoindex_ = rhs.autoindex_;
  this->serverConfigs_ = rhs.serverConfigs_;
  this->errorPages_ = rhs.errorPages_;
  return *this;
}

RootConfig::~RootConfig() {}

void RootConfig::printRootConfig() {
  std::vector<ServerConfig>::iterator server;
  Log::debug << "root: " << this->rootPath_ << NL;
  Log::debug << "client_max_body_size: " << this->limitClientBodySize_ << NL;
  Log::debug << "autoindex: " << std::boolalpha << autoindex_ << NL;
  std::map<int, std::string>::const_iterator errorPage;
  for (errorPage = errorPages_.begin(); errorPage != errorPages_.end();
       errorPage++) {
    std::stringstream ss;
    ss << errorPage->first;
    Log::debug << "error_page: " << ss.str() << " " << errorPage->second << NL;
  }
  for (server = this->serverConfigs_.begin();
       server != this->serverConfigs_.end(); server++) {
    server->printServerConfig();
  }
}

std::string RootConfig::getRootPath() const { return rootPath_; }

void RootConfig::setRootPath(const std::string &rootPath) {
  rootPath_ = rootPath;
}

int RootConfig::getLimitClientBodySize() const { return limitClientBodySize_; }

void RootConfig::setLimitClientBodySize(
    const std::string &limitClientBodySize) {
  std::stringstream ss;
  ss << limitClientBodySize;
  ss >> limitClientBodySize_;
  limitClientBodySize_ = limitClientBodySize_ * 1024 * 1024;
}

void RootConfig::setAutoindex(const std::string &autoindex) {
  if (autoindex == "on") {
    autoindex_ = true;
  }
}

bool RootConfig::getAutoindex() const { return autoindex_; }

std::vector<ServerConfig> &RootConfig::getServerConfigs() {
  return serverConfigs_;
}

void RootConfig::setServerConfigs(
    const std::vector<ServerConfig> &serverConfigs) {
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

void RootConfig::addErrorPage(int errorCode, const std::string &page) {
  errorPages_.insert(std::pair<int, std::string>(errorCode, page));
}

const std::string RootConfig::getErrorPage(int errorCode) const {
  if (errorPages_.find(errorCode) == errorPages_.end()) {
    return "";
  }
  return errorPages_.at(errorCode);
}

const std::map<int, std::string> &RootConfig::getErrorPages() const {
  return errorPages_;
}
