#ifndef CONFIG_HPP_
#define CONFIG_HPP_

#include <iostream>
#include <map>
#include <vector>

#include "Directive.hpp"

class LocationConfig {
public:
  LocationConfig();

  std::string uri;
  std::vector<std::string> acceptMethods;
  std::string rootPath;

  int redirectionStatusCode;
  std::string redirectionPath;

  bool autoIndex; // default false
  std::string indexFile;

  std::map<std::string, std::string> cgiPrograms;
};

class ServerConfig {
public:
  ServerConfig();

  int port;
  int limitClientBodySize;
  std::string rootPath;
  std::string serverName;
  std::map<int, std::string> errorPages;

  std::vector<LocationConfig> locationConfigs;
};

class RootConfig {
public:
  RootConfig();
  RootConfig(Directive Directive);
  RootConfig(const RootConfig &src);
  RootConfig &operator=(const RootConfig &rhs);
  ~RootConfig();

  void FillInConfig(void);

  std::vector<ServerConfig> serverConfigs;

private:
};

#endif
