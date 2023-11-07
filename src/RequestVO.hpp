#ifndef RequestVO_HPP_
#define RequestVO_HPP_

#include <string>
#include <map>

class RequestVO {
public:
  const std::string &getUri() const;
  void setUri(const std::string &uri);
  const std::string &getMethod() const;
  void setMethod(const std::string &method);
  const std::string &getVersion() const;
  void setVersion(const std::string &version);
  const std::string &getBody() const;
  void setBody(const std::string &body);
  void appendBody(const std::string &body);
  const std::map<std::string, std::string> &getHeaders() const;
  void setHeaders(const std::map<std::string, std::string> &headers);
  void setHeader(const std::string &key, const std::string &value);
  bool hasHeader(const std::string &key) const;
  const std::string &getHeader(const std::string &key) const;

private:
  std::string uri_;
  std::string method_;
  std::string version_;
  std::string body_;
  std::map<std::string, std::string> headers_;

};

#endif
