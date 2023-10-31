#ifndef SEVERCONFIG_HPP_
#define SEVERCONFIG_HPP_

#include <iostream>
#include <map>
#include <vector>

#include "LocationConfig.hpp"

class RootConfig;
class ServerConfig {
public:
  ServerConfig(const RootConfig &rootConfig);
  ServerConfig(const ServerConfig &src);
  ServerConfig &operator=(const ServerConfig &rhs);
  ~ServerConfig();

  void printServerConfig();

  int getPort() const;
  void setPort(const int &port);
  int getLimitClientBodySize() const;
  void setLimitClientBodySize(const int &limitClientBodySize);
  std::string getRootPath() const;
  void setRootPath(const std::string &rootPath);
  std::string getServerName() const;
  void setServerName(const std::string &serverName);
  std::map<int, std::string> getErrorPages() const;
  void setErrorPages(const std::map<int, std::string> &errorPages);
  void addErrorPages(const std::pair<int, std::string> &errorPages);
  std::vector<LocationConfig> getLocationConfigs() const;
  void setLocationConfigs(const std::vector<LocationConfig> &locationConfigs);
  void addLocationConfigs(const LocationConfig &locationConfigs);
  const std::vector<LocationConfig>::iterator beginLocationConfigs();
  const std::vector<LocationConfig>::iterator endLocationConfigs();

private:
  int port_;
  int limitClientBodySize_;
  std::string rootPath_;
  std::string serverName_;
  std::map<int, std::string> errorPages_;
  std::vector<LocationConfig> locationConfigs_;
};

#endif
