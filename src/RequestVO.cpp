#include "RequestVO.hpp"

const std::string &RequestVO::getUri() const { return uri_; }

void RequestVO::setUri(const std::string &uri) { uri_ = uri; }

const std::string &RequestVO::getMethod() const { return method_; }

void RequestVO::setMethod(const std::string &method) { method_ = method; }

const std::string &RequestVO::getVersion() const { return version_; }

void RequestVO::setVersion(const std::string &version) { version_ = version; }

const std::string &RequestVO::getBody() const { return body_; }

void RequestVO::setBody(const std::string &body) { body_ = body; }

void RequestVO::appendBody(const std::string &body) { body_ += body; }

const std::map<std::string, std::string> &RequestVO::getHeaders() const {
  return headers_;
}

void RequestVO::setHeaders(const std::map<std::string, std::string> &headers) {
  headers_ = headers;
}

void RequestVO::setHeader(const std::string &key, const std::string &value) {
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

bool RequestVO::hasHeader(const std::string &key) const {
  return headers_.find(key) != headers_.end();
}

const std::string &RequestVO::getHeader(const std::string &key) const {
  return headers_.at(key);
}
