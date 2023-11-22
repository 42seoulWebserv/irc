#include "ServerConfig.hpp"

#include <sstream>

#include "RootConfig.hpp"

ServerConfig::ServerConfig(const RootConfig &src)
    : port_(80), limitClientBodySize_(0) {
  this->rootPath_ = src.getRootPath();
  this->limitClientBodySize_ = src.getLimitClientBodySize();
  this->errorPages_ = src.getErrorPages();
}

ServerConfig::ServerConfig(const ServerConfig &src) { *this = src; }

ServerConfig &ServerConfig::operator=(const ServerConfig &rhs) {
  if (this == &rhs) {
    return *this;
  }
  this->port_ = rhs.port_;
  this->limitClientBodySize_ = rhs.limitClientBodySize_;
  this->rootPath_ = rhs.rootPath_;
  this->serverName_ = rhs.serverName_;
  this->errorPages_ = rhs.errorPages_;
  this->locationConfigs_ = rhs.locationConfigs_;
  this->errorPages_ = rhs.errorPages_;
  return *this;
}

ServerConfig::~ServerConfig(void) {}

void ServerConfig::printServerConfig(void) {
  std::cout << "server {" << '\n';
  std::cout << "  root: " << this->rootPath_ << '\n';
  std::cout << "  server_name: " << this->serverName_ << '\n';
  std::cout << "  client_max_body_size: " << this->limitClientBodySize_ << '\n';
  std::cout << "  listen: " << this->port_ << '\n';
  std::map<int, std::string>::const_iterator errorPage;
  for (errorPage = errorPages_.begin(); errorPage != errorPages_.end();
       errorPage++) {
    std::stringstream ss;
    ss << errorPage->first;
    std::cout << "  error_page: " << ss.str() << " " << errorPage->second
              << '\n';
  }
  std::vector<LocationConfig>::iterator location;
  for (location = this->locationConfigs_.begin();
       location != this->locationConfigs_.end(); location++) {
    location->printLocationConfig();
  }
  std::cout << '}' << '\n';
}

int ServerConfig::getPort() const { return port_; }

void ServerConfig::setPort(const int &port) { port_ = port; }

int ServerConfig::getLimitClientBodySize() const {
  return limitClientBodySize_;
}

void ServerConfig::setLimitClientBodySize(const int &limitClientBodySize) {
  limitClientBodySize_ = limitClientBodySize;
}

std::string ServerConfig::getRootPath() const { return rootPath_; }

void ServerConfig::setRootPath(const std::string &rootPath) {
  rootPath_ = rootPath;
}

std::string ServerConfig::getServerName() const { return serverName_; }

void ServerConfig::setServerName(const std::string &serverName) {
  serverName_ = serverName;
}

std::string ServerConfig::getIndex() const { return index_; }

void ServerConfig::setIndex(const std::string &index) { index_ = index; }

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
