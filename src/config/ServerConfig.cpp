#include "ServerConfig.hpp"

#include <sstream>

#include "Log.hpp"
#include "RootConfig.hpp"

ServerConfig::ServerConfig(const RootConfig &src)
    : autoindex_(false), port_(80), limitClientBodySize_(0) {
  this->indexPath_ = src.getIndexPath();
  this->rootPath_ = src.getRootPath();
  this->autoindex_ = src.getAutoindex();
  this->limitClientBodySize_ = src.getLimitClientBodySize();
  this->errorPages_ = src.getErrorPages();
}

ServerConfig::ServerConfig(const ServerConfig &src) { *this = src; }

ServerConfig &ServerConfig::operator=(const ServerConfig &rhs) {
  if (this == &rhs) {
    return *this;
  }
  this->autoindex_ = rhs.autoindex_;
  this->port_ = rhs.port_;
  this->limitClientBodySize_ = rhs.limitClientBodySize_;
  this->rootPath_ = rhs.rootPath_;
  this->serverName_ = rhs.serverName_;
  this->indexPath_ = rhs.indexPath_;
  this->errorPages_ = rhs.errorPages_;
  this->locationConfigs_ = rhs.locationConfigs_;
  this->errorPages_ = rhs.errorPages_;
  return *this;
}

ServerConfig::~ServerConfig(void) {}

void ServerConfig::printServerConfig(void) {
  Log::debug << "server {" << NL;
  Log::debug << "  root: " << this->rootPath_ << NL;
  Log::debug << "  server_name: " << this->serverName_ << NL;
  Log::debug << "  client_max_body_size: " << this->limitClientBodySize_ << NL;
  Log::debug << "  listen: " << this->port_ << NL;
  Log::debug << "  autoindex: " << std::boolalpha << this->autoindex_ << NL;
  Log::debug << "  index: " << this->indexPath_ << NL;
  std::map<int, std::string>::const_iterator errorPage;
  for (errorPage = errorPages_.begin(); errorPage != errorPages_.end();
       errorPage++) {
    std::stringstream ss;
    ss << errorPage->first;
    Log::debug << "  error_page: " << ss.str() << " " << errorPage->second
               << NL;
  }
  std::vector<LocationConfig>::iterator location;
  for (location = locationConfigs_.begin(); location != locationConfigs_.end();
       location++) {
    location->printLocationConfig();
  }
  Log::debug << '}' << NL;
}

void ServerConfig::setAutoindex(const std::string &autoindex) {
  if (autoindex == "on") {
    autoindex_ = true;
  }
}

bool ServerConfig::getAutoindex() const { return autoindex_; }

int ServerConfig::getPort() const { return port_; }

void ServerConfig::setPort(const int &port) { port_ = port; }

int ServerConfig::getLimitClientBodySize() const {
  return limitClientBodySize_;
}

void ServerConfig::setLimitClientBodySize(
    const std::string &limitClientBodySize) {
  std::stringstream ss;
  ss << limitClientBodySize;
  ss >> limitClientBodySize_;
  limitClientBodySize_ = limitClientBodySize_ * 1024 * 1024;
}

std::string ServerConfig::getRootPath() const { return rootPath_; }

void ServerConfig::setRootPath(const std::string &rootPath) {
  rootPath_ = rootPath;
}

std::string ServerConfig::getServerName() const { return serverName_; }

void ServerConfig::setServerName(const std::string &serverName) {
  serverName_ = serverName;
}

std::string ServerConfig::getIndexPath() const { return indexPath_; }

void ServerConfig::setIndexPath(const std::string &indexPath) {
  indexPath_ = indexPath;
}

void ServerConfig::setErrorPages(const std::map<int, std::string> &errorPages) {
  errorPages_ = errorPages;
}

void ServerConfig::addErrorPages(
    const std::pair<int, std::string> &errorPages) {
  errorPages_.insert(errorPages);
}

const std::vector<LocationConfig> &ServerConfig::getLocationConfigs() const {
  return locationConfigs_;
}

void ServerConfig::setLocationConfigs(
    const std::vector<LocationConfig> &locationConfigs) {
  locationConfigs_ = locationConfigs;
}

void ServerConfig::addLocationConfigs(const LocationConfig &locationConfigs) {
  locationConfigs_.push_back(locationConfigs);
}

const std::vector<LocationConfig>::iterator
ServerConfig::beginLocationConfigs() {
  return locationConfigs_.begin();
}

const std::vector<LocationConfig>::iterator ServerConfig::endLocationConfigs() {
  return locationConfigs_.end();
}

void ServerConfig::addErrorPage(int errorCode, const std::string &page) {
  errorPages_.insert(std::pair<int, std::string>(errorCode, page));
}

const std::string ServerConfig::getErrorPage(int errorCode) const {
  if (errorPages_.find(errorCode) == errorPages_.end()) {
    return "";
  }
  return errorPages_.at(errorCode);
}

const std::map<int, std::string> &ServerConfig::getErrorPages() const {
  return errorPages_;
}
