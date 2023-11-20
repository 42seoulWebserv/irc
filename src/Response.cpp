#include "Response.hpp"

#include <sstream>

const std::string &Response::getVersion() const { return version_; }

void Response::setVersion(const std::string &version) { version_ = version; }

int Response::getStatusCode() const { return statusCode_; }

void Response::setStatusCode(const int statusCode) {
  statusCode_ = statusCode;
}

const std::map<std::string, std::string> &Response::getHeaders() const {
  return headers_;
}

void Response::setHeaders(const std::map<std::string, std::string> &headers) {
  headers_ = headers;
}

void Response::setHeader(const std::string &key, const std::string &value) {
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

bool Response::hasHeader(const std::string &key) const {
  return headers_.find(key) != headers_.end();
}

const std::string &Response::getHeader(const std::string &key) const {
  return headers_.at(key);
}

std::string Response::toString() const {
  std::stringstream ss;
  ss << version_ << " " << statusCode_ << " "
     << "!!!OK!!!"
     << "\r\n";
  std::map<std::string, std::string>::const_iterator iter;
  for (iter = headers_.begin(); iter != headers_.end(); iter++) {
    ss << iter->first << ": " << iter->second << "\r\n";
  }
  ss << "\r\n";
  return ss.str();
}
