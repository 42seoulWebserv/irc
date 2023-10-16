#ifndef ROOTCONFIG_HPP_
#define ROOTCONFIG_HPP_

#include <iostream>
#include <map>
#include <vector>

#include "ServerConfig.hpp"

class RootConfig {
public:
  RootConfig();
  RootConfig(const RootConfig &src);
  RootConfig &operator=(const RootConfig &rhs);
  ~RootConfig();

  void printRootConfig();

  std::vector<ServerConfig> getServerConfigs() const;
  void setServerConfigs(const std::vector<ServerConfig> &serverConfigs);
  void addServerConfigs(const ServerConfig &serverConfigs);
  const std::vector<ServerConfig>::iterator beginServerConfigs();
  const std::vector<ServerConfig>::iterator endServerConfigs();

private:
  std::vector<ServerConfig> serverConfigs_;
};

#endif
