#include "ServerConfig.hpp"

#include <sstream>

#include "Log.hpp"
#include "RootConfig.hpp"

ServerConfig::ServerConfig(const RootConfig &src)
    : rootPath_(src.getRootPath()),
      port_(80),
      indexPath_(src.getIndexPath()),
      redirectionStatusCode_(0),
      clientMaxBodySize_(src.getClientMaxBodySize()),
      errorPages_(src.getErrorPages()),
      autoindex_(src.getAutoindex()) {}

ServerConfig::ServerConfig(const ServerConfig &src) { *this = src; }

ServerConfig &ServerConfig::operator=(const ServerConfig &rhs) {
  if (this == &rhs) {
    return *this;
  }
  rootPath_ = rhs.rootPath_;
  port_ = rhs.port_;
  serverName_ = rhs.serverName_;
  redirectionStatusCode_ = rhs.redirectionStatusCode_;
  redirectionPath_ = rhs.redirectionPath_;
  indexPath_ = rhs.indexPath_;
  clientMaxBodySize_ = rhs.clientMaxBodySize_;
  errorPages_ = rhs.errorPages_;
  autoindex_ = rhs.autoindex_;
  locationConfigs_ = rhs.locationConfigs_;
  return *this;
}

ServerConfig::~ServerConfig(void) {}

void ServerConfig::printServerConfig(void) {
  Log::debug << "server {" << NL;
  Log::debug << "  root: " << rootPath_ << NL;
  Log::debug << "  listen: " << port_ << NL;
  Log::debug << "  server_name: " << serverName_ << NL;
  Log::debug << "  return: " << redirectionStatusCode_ << ' '
             << redirectionPath_ << NL;
  Log::debug << "  index: " << indexPath_ << NL;
  Log::debug << "  client_max_body_size: " << clientMaxBodySize_ << NL;
  std::map<int, std::string>::const_iterator errorPage;
  for (errorPage = errorPages_.begin(); errorPage != errorPages_.end();
       errorPage++) {
    std::stringstream ss;
    ss << errorPage->first;
    Log::debug << "  error_page: " << ss.str() << " " << errorPage->second
               << NL;
  }
  Log::debug << "  autoindex: " << std::boolalpha << autoindex_ << NL;
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

int ServerConfig::getClientMaxBodySize() const { return clientMaxBodySize_; }

void ServerConfig::setClientMaxBodySize(
    const std::string &limitClientBodySize) {
  std::stringstream ss;
  ss << limitClientBodySize;
  ss >> clientMaxBodySize_;
  clientMaxBodySize_ = clientMaxBodySize_ * 1024 * 1024;
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

int ServerConfig::getRedirectionStatusCode() const {
  return redirectionStatusCode_;
}

void ServerConfig::setRedirectionStatusCode(int statusCode) {
  redirectionStatusCode_ = statusCode;
}

const std::string &ServerConfig::getRedirectionPath() const {
  return redirectionPath_;
}

void ServerConfig::setRedirectionPath(const std::string &path) {
  redirectionPath_ = path;
}
