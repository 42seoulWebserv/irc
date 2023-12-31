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
  std::string getIndexPath() const;
  void setIndexPath(const std::string &indexPath);
  int getClientMaxBodySize() const;
  void setClientMaxBodySize(const std::string &limitClientBodySize);
  void setAutoindex(const std::string &autoindex);
  bool getAutoindex() const;
  std::vector<ServerConfig> &getServerConfigs();
  void setServerConfigs(const std::vector<ServerConfig> &serverConfigs);
  void addServerConfigs(const ServerConfig &serverConfigs);
  const std::vector<ServerConfig>::iterator beginServerConfigs();
  const std::vector<ServerConfig>::iterator endServerConfigs();
  void addErrorPage(int errorCode, const std::string &page);
  const std::string getErrorPage(int errorCode) const;
  const std::map<int, std::string> &getErrorPages() const;

 private:
  std::string rootPath_;
  std::string indexPath_;
  long long clientMaxBodySize_;
  std::map<int, std::string> errorPages_;
  bool autoindex_;
  std::vector<ServerConfig> serverConfigs_;
};

#endif
