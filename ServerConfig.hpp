#ifndef SEVERCONFIG_HPP_
#define SEVERCONFIG_HPP_

#include <iostream>
#include <map>
#include <vector>

#include "LocationConfig.hpp"

class ServerConfig {
public:
  ServerConfig();
  ServerConfig(const ServerConfig &src);
  ServerConfig &operator=(const ServerConfig &rhs);
  ~ServerConfig();

  void printServerConfig();

private:
  int port_;
  int limitClientBodySize_;
  std::string rootPath_;
  std::string serverName_;
  std::map<int, std::string> errorPages_;
  std::vector<LocationConfig> locationConfigs_;
};

#endif
