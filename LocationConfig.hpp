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
  const std::string &setUri() const;

private:
  bool autoIndex_; // default false
  int redirectionStatusCode_;
  std::string uri_;
  std::string rootPath_;
  std::string indexPath_;
  std::string redirectionPath_;
  std::vector<std::string> acceptMethods_;
  std::map<std::string, std::string> cgiPrograms_;
};

#endif
