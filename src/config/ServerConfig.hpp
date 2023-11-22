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
  std::string getIndex() const;
  void setIndex(const std::string &index);
  void setErrorPages(const std::map<int, std::string> &errorPages);
  void addErrorPages(const std::pair<int, std::string> &errorPages);
  const std::vector<LocationConfig> &getLocationConfigs() const;
  void setLocationConfigs(const std::vector<LocationConfig> &locationConfigs);
  void addLocationConfigs(const LocationConfig &locationConfigs);
  const std::vector<LocationConfig>::iterator beginLocationConfigs();
  const std::vector<LocationConfig>::iterator endLocationConfigs();
  void addErrorPage(int errorCode, const std::string &page);
  const std::string getErrorPage(int errorCode) const;
  const std::map<int, std::string> &getErrorPages() const;

 private:
  int port_;
  int limitClientBodySize_;
  std::string rootPath_;
  std::string serverName_;
  std::string index_;
  std::vector<LocationConfig> locationConfigs_;
  std::map<int, std::string> errorPages_;
};

#endif
