#ifndef LOCATIONCONFIG_HPP_
#define LOCATIONCONFIG_HPP_

#include <iostream>
#include <map>
#include <vector>

class LocationConfig {
public:
  LocationConfig();
  LocationConfig(const LocationConfig &src);
  LocationConfig &operator=(const LocationConfig &rhs);
  ~LocationConfig();

  void printLocationConfig(void);

  const std::string &getUri() const;

  std::string uri_;
  std::vector<std::string> acceptMethods_;
  std::string rootPath_;

  int redirectionStatusCode_;
  std::string redirectionPath_;

  bool autoIndex_; // default false
  std::string indexPath_;

  std::map<std::string, std::string> cgiPrograms_;

private:
};

#endif
