#include "Response.hpp"

#include <sstream>

std::map<int, std::string> Response::statusMap_;

const std::string &Response::getVersion() const { return version_; }

void Response::setVersion(const std::string &version) { version_ = version; }

int Response::getStatusCode() const { return statusCode_; }

std::string Response::getStatusMsg() const {
  if (statusMap_.size() == 0) {
    createStatusMap();
  }
  if (statusMap_.find(statusCode_) == statusMap_.end()) {
    return "";
  }
  return statusMap_[statusCode_];
}

void Response::setStatusCode(const int statusCode) { statusCode_ = statusCode; }

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
  ss << version_ << " " << statusCode_ << " " << getStatusMsg() << "\r\n";
  std::map<std::string, std::string>::const_iterator iter;
  for (iter = headers_.begin(); iter != headers_.end(); iter++) {
    ss << iter->first << ": " << iter->second << "\r\n";
  }
  ss << "\r\n";
  return ss.str();
}

void Response::createStatusMap() {
  statusMap_[100] = "Continue";
  statusMap_[101] = "Switching Protocols";
  statusMap_[102] = "Processing";
  statusMap_[103] = "Early Hints";

  statusMap_[200] = "OK";
  statusMap_[201] = "Created";
  statusMap_[202] = "Accepted";
  statusMap_[203] = "Non-Authoritative Information";
  statusMap_[204] = "No Content";
  statusMap_[205] = "Reset Content";
  statusMap_[206] = "Partial Content";
  statusMap_[207] = "Multi-Status";
  statusMap_[208] = "Already Reported";
  statusMap_[226] = "IM Used";

  statusMap_[300] = "Multiple Choices";
  statusMap_[301] = "Moved Permanently";
  statusMap_[302] = "Found";
  statusMap_[303] = "See Other";
  statusMap_[304] = "Not Modified";
  statusMap_[305] = "Use Proxy";
  statusMap_[306] = "Switch Proxy";
  statusMap_[307] = "Temporary Redirect";
  statusMap_[308] = "Permanent Redirect";

  statusMap_[400] = "Bad Request";
  statusMap_[401] = "Unauthorized";
  statusMap_[402] = "Payment Required";
  statusMap_[403] = "Forbidden";
  statusMap_[404] = "Not Found";
  statusMap_[405] = "Method Not Allowed";
  statusMap_[406] = "Not Acceptable";
  statusMap_[407] = "Proxy Authentication Required";
  statusMap_[408] = "Request Timeout";
  statusMap_[409] = "Conflict";
  statusMap_[410] = "Gone";
  statusMap_[411] = "Length Required";
  statusMap_[412] = "Precondition Failed";
  statusMap_[413] = "Payload Too Large";
  statusMap_[414] = "URI Too Long";
  statusMap_[415] = "Unsupported Media Type";
  statusMap_[416] = "Range Not Satisfiable";
  statusMap_[417] = "Expectation Failed";
  statusMap_[418] = "I'm a teapot";
  statusMap_[421] = "Misdirected Request";
  statusMap_[422] = "Unprocessable Entity";
  statusMap_[423] = "Locked";
  statusMap_[424] = "Failed Dependency";
  statusMap_[425] = "Too Early";
  statusMap_[426] = "Upgrade Required";
  statusMap_[428] = "Precondition Required";
  statusMap_[429] = "Too Many Requests";
  statusMap_[431] = "Request Header Fields Too Large";
  statusMap_[451] = "Unavailable For Legal Reasons";

  statusMap_[500] = "Internal Server Error";
  statusMap_[501] = "Not Implemented";
  statusMap_[502] = "Bad Gateway";
  statusMap_[503] = "Service Unavailable";
  statusMap_[504] = "Gateway Timeout";
  statusMap_[505] = "HTTP Version Not Supported";
  statusMap_[506] = "Variant Also Negotiates";
  statusMap_[507] = "Insufficient Storage";
  statusMap_[508] = "Loop Detected";
  statusMap_[510] = "Not Extended";
  statusMap_[511] = "Network Authentication Required";
}