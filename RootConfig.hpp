#ifndef ROOTCONFIG_HPP_
#define ROOTCONFIG_HPP_

#include <iostream>
#include <map>
#include <vector>

#include "ServerConfig.hpp"

class RootConfig {
public:
  RootConfig();
  // RootConfig(Directive Directive);
  RootConfig(const RootConfig &src);
  RootConfig &operator=(const RootConfig &rhs);
  ~RootConfig();

  void FillInConfig(void);
  void printRootConfig(void);

  std::vector<ServerConfig> serverConfigs_;
private:
};

#endif
