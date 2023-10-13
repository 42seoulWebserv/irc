#ifndef CONFIG_HPP_
#define CONFIG_HPP_

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

  void printServerConfig(void);

  int port_;
  int limitClientBodySize_;
  std::string rootPath_;
  std::string serverName_;
  std::map<int, std::string> errorPages_;

  std::vector<LocationConfig> locationConfigs_;
private:
};

#endif
