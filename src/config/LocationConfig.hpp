#ifndef LOCATIONCONFIG_HPP_
#define LOCATIONCONFIG_HPP_

#include <iostream>
#include <map>
#include <vector>

class ServerConfig;
class LocationConfig {
 public:
  LocationConfig(const ServerConfig &src);
  LocationConfig(const LocationConfig &src);
  LocationConfig &operator=(const LocationConfig &rhs);
  ~LocationConfig();

  void printLocationConfig(void);

  int getClientMaxBodySize() const;
  void setClientMaxBodySize(const std::string &limitClientBodySize);
  bool getAutoindex() const;
  void setAutoindex(const std::string &autoindex);
  int getRedirectionStatusCode() const;
  void setRedirectionStatusCode(const int &redirectionStatusCode);
  std::string getUri() const;
  void setUri(const std::string &uri);
  std::string getRootPath() const;
  void setRootPath(const std::string &rootPath);
  std::string getIndexPath() const;
  void setIndexPath(const std::string &indexPath);
  std::string getRedirectionPath() const;
  void setRedirectionPath(const std::string &redirectionPath);
  std::vector<std::string> getAcceptMethods() const;
  void setAcceptMethods(const std::vector<std::string> &acceptMethods);
  void addAcceptMethods(const std::string &acceptMethods);
  const std::vector<std::string>::iterator beginAcceptMethods();
  const std::vector<std::string>::iterator endAcceptMethods();
  const std::string getElementAtIndexAcceptMethods(size_t index) const;
  std::map<std::string, std::string> getCgiPrograms() const;
  void setCgiPrograms(const std::map<std::string, std::string> &cgiPrograms);
  void addCgiPrograms(const std::pair<std::string, std::string> &cgiPrograms);
  const std::map<std::string, std::string>::iterator beginCgiPrograms();
  const std::map<std::string, std::string>::iterator endCgiPrograms();
  bool hasCgiProgram(const std::string &key) const;
  const std::string &getCgiProgram(const std::string &key) const;
  void addErrorPage(int errorCode, const std::string &page);
  const std::string getErrorPage(int errorCode) const;
  const std::map<int, std::string> &getErrorPages() const;

 private:
  std::string uri_;
  std::string rootPath_;
  int redirectionStatusCode_;
  std::string redirectionPath_;
  std::string indexPath_;
  long long clientMaxBodySize_;
  std::map<int, std::string> errorPages_;
  bool autoindex_;
  std::vector<std::string> acceptMethods_;
  std::map<std::string, std::string> cgiPrograms_;
};

#endif
