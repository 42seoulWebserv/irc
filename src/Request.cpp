#include "Request.hpp"

const std::string &Request::getUri() const { return uri_; }

void Request::setUri(const std::string &uri) { uri_ = uri; }

const std::string &Request::getMethod() const { return method_; }

void Request::setMethod(const std::string &method) { method_ = method; }

const std::string &Request::getVersion() const { return version_; }

void Request::setVersion(const std::string &version) { version_ = version; }

const std::string &Request::getBody() const { return body_; }

void Request::setBody(const std::string &body) { body_ = body; }

void Request::appendBody(const std::string &body) { body_ += body; }

const std::map<std::string, std::string> &Request::getHeaders() const {
  return headers_;
}

void Request::setHeaders(const std::map<std::string, std::string> &headers) {
  headers_ = headers;
}

void Request::setHeader(const std::string &key, const std::string &value) {
  bool upper = true;
  std::string keyCopy = key;
  for (size_t i = 0; i < keyCopy.size(); i++) {
    if (upper) {
      keyCopy[i] = std::toupper(keyCopy[i]);
    } else {
      keyCopy[i] = std::tolower(keyCopy[i]);
    }
    upper = keyCopy[i] == '-';
  }
  headers_[keyCopy] = value;
}

bool Request::hasHeader(const std::string &key) const {
  return headers_.find(key) != headers_.end();
}

const std::string &Request::getHeader(const std::string &key) const {
  return headers_.at(key);
}
