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

  std::string getRootPath() const;
  void setRootPath(const std::string &rootPath);
  int getLimitClientBodySize() const;
  void setLimitClientBodySize(const int &limitClientBodySize);
  std::vector<ServerConfig> &getServerConfigs();
  void setServerConfigs(const std::vector<ServerConfig> &serverConfigs);
  void addServerConfigs(const ServerConfig &serverConfigs);
  const std::vector<ServerConfig>::iterator beginServerConfigs();
  const std::vector<ServerConfig>::iterator endServerConfigs();

 private:
  std::string rootPath_;
  int limitClientBodySize_;
  std::vector<ServerConfig> serverConfigs_;
};

#endif
