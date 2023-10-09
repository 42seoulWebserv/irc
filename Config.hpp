#ifndef CONFIG_HPP_
#define CONFIG_HPP_

#include <iostream>
#include <map>
#include <vector>

#include "BlockDirective.hpp"

class RootConfig {
public:
  RootConfig(BlockDirective Directive);
  RootConfig(const RootConfig &src);
  RootConfig &operator=(const RootConfig &rhs);
  ~RootConfig(void);

  void FillInConfig(void);

  std::vector<ServerConfig> serverConfigs;

private:
  RootConfig(void);
};

class ServerConfig {
  int port;
  int limitClientBodySize;
  std::string rootPath;
  std::string serverName;
  std::map<int, std::string> errorPages;

  std::vector<LocationConfig> locationConfigs;
};

class LocationConfig {
  std::string uri;
  std::vector<std::string> acceptMethods;
  std::string rootPath;

  int redirectionStatusCode;
  std::string redirectionPath;

  bool autoIndex; // default false
  std::string indexFile;

  std::map<std::string, std::string> cgiPrograms;
};

#endif
