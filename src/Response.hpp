#ifndef Response_HPP_
#define Response_HPP_

#include <map>
#include <string>

class Response {
 public:
  const std::string &getVersion() const;
  void setVersion(const std::string &version);
  int getStatusCode() const;
  std::string getStatusMsg() const;
  static std::string getStatusMsg(int code);
  void setStatusCode(const int statusCode);
  const std::map<std::string, std::string> &getHeaders() const;
  void setHeaders(const std::map<std::string, std::string> &headers);
  void setHeader(const std::string &key, const std::string &value);
  bool hasHeader(const std::string &key) const;
  const std::string &getHeader(const std::string &key) const;
  std::string toString() const;
  static void createStatusMap();

 private:
  std::string version_;
  int statusCode_;
  std::map<std::string, std::string> headers_;
  static std::map<int, std::string> statusMap_;
};

#endif
