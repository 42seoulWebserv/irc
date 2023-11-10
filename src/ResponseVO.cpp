#include "ResponseVO.hpp"

const std::string &ResponseVO::getVersion() const { return version_; }

void ResponseVO::setVersion(const std::string &version) { version_ = version; }

const int ResponseVO::getStatusCode() const { return statusCode_; }

void ResponseVO::setStatusCode(const int statusCode) {
  statusCode_ = statusCode;
}

const std::string &ResponseVO::getBody() const { return body_; }

void ResponseVO::setBody(const std::string &body) { body_ = body; }

void ResponseVO::appendBody(const std::string &body) { body_ += body; }

const std::map<std::string, std::string> &ResponseVO::getHeaders() const {
  return headers_;
}

void ResponseVO::setHeaders(const std::map<std::string, std::string> &headers) {
  headers_ = headers;
}

void ResponseVO::setHeader(const std::string &key, const std::string &value) {
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

bool ResponseVO::hasHeader(const std::string &key) const {
  return headers_.find(key) != headers_.end();
}

const std::string &ResponseVO::getHeader(const std::string &key) const {
  return headers_.at(key);
}
