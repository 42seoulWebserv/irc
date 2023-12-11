#include "LocationConfig.hpp"

#include <sstream>

#include "Log.hpp"
#include "ServerConfig.hpp"

LocationConfig::LocationConfig(const ServerConfig &src)
    : limitClientBodySize_(0),
      autoindex_(false),
      redirectionStatusCode_(0),
      indexPath_("index.html") {
  this->rootPath_ = src.getRootPath();
  this->autoindex_ = src.getAutoindex();
  this->limitClientBodySize_ = src.getLimitClientBodySize();
  this->errorPages_ = src.getErrorPages();
}

LocationConfig::LocationConfig(const LocationConfig &src) { *this = src; }

LocationConfig &LocationConfig::operator=(const LocationConfig &rhs) {
  if (this == &rhs) {
    return *this;
  }
  this->limitClientBodySize_ = rhs.limitClientBodySize_;
  this->autoindex_ = rhs.autoindex_;
  this->redirectionStatusCode_ = rhs.redirectionStatusCode_;
  this->uri_ = rhs.uri_;
  this->rootPath_ = rhs.rootPath_;
  this->indexPath_ = rhs.indexPath_;
  this->redirectionPath_ = rhs.redirectionPath_;
  this->acceptMethods_ = rhs.acceptMethods_;
  this->cgiPrograms_ = rhs.cgiPrograms_;
  this->errorPages_ = rhs.errorPages_;
  return *this;
}

LocationConfig::~LocationConfig(void) {}

void LocationConfig::printLocationConfig(void) {
  Log::debug << "  location " << uri_ << " {" << NL;
  Log::debug << "    root: " << rootPath_ << NL;
  Log::debug << "    client_max_body_size: " << limitClientBodySize_ << NL;
  Log::debug << "    return " << redirectionStatusCode_ << ' '
             << redirectionPath_ << NL;
  Log::debug << "    index: " << indexPath_ << NL;
  Log::debug << "    autoindex: " << std::boolalpha << autoindex_ << NL;
  Log::debug << "    accept_methods ";
  std::vector<std::string>::iterator method;
  for (method = acceptMethods_.begin(); method != acceptMethods_.end();
       method++) {
    Log::debug << *method << ' ';
  }
  Log::debug << NL;
  std::map<std::string, std::string>::iterator cgi;
  for (cgi = this->cgiPrograms_.begin(); cgi != this->cgiPrograms_.end();
       cgi++) {
    Log::debug << "    cgi_extension " << cgi->first << ' ' << cgi->second
               << NL;
  }
  std::map<int, std::string>::const_iterator errorPage;
  for (errorPage = errorPages_.begin(); errorPage != errorPages_.end();
       errorPage++) {
    std::stringstream ss;
    ss << errorPage->first;
    Log::debug << "    error_page: " << ss.str() << " " << errorPage->second
               << NL;
  }
  Log::debug << "  }" << NL;
}

int LocationConfig::getLimitClientBodySize() const {
  return limitClientBodySize_;
}

void LocationConfig::setLimitClientBodySize(
    const std::string &limitClientBodySize) {
  std::stringstream ss;
  ss << limitClientBodySize;
  ss >> limitClientBodySize_;
  limitClientBodySize_ = limitClientBodySize_ * 1024 * 1024;
}

bool LocationConfig::getAutoindex() const { return autoindex_; }

void LocationConfig::setAutoindex(const std::string &autoindex) {
  if (autoindex == "on") {
    autoindex_ = true;
  }
}

int LocationConfig::getRedirectionStatusCode() const {
  return redirectionStatusCode_;
}

void LocationConfig::setRedirectionStatusCode(
    const int &redirectionStatusCode) {
  redirectionStatusCode_ = redirectionStatusCode;
}

std::string LocationConfig::getUri() const { return uri_; }

void LocationConfig::setUri(const std::string &uri) { uri_ = uri; }

std::string LocationConfig::getRootPath() const { return rootPath_; }

void LocationConfig::setRootPath(const std::string &rootPath) {
  rootPath_ = rootPath;
}

std::string LocationConfig::getIndexPath() const { return indexPath_; }

void LocationConfig::setIndexPath(const std::string &indexPath) {
  indexPath_ = indexPath;
}

std::string LocationConfig::getRedirectionPath() const {
  return redirectionPath_;
}

void LocationConfig::setRedirectionPath(const std::string &redirectionPath) {
  redirectionPath_ = redirectionPath;
}

std::vector<std::string> LocationConfig::getAcceptMethods() const {
  return acceptMethods_;
}

void LocationConfig::setAcceptMethods(
    const std::vector<std::string> &acceptMethods) {
  acceptMethods_ = acceptMethods;
}

void LocationConfig::addAcceptMethods(const std::string &acceptMethods) {
  acceptMethods_.push_back(acceptMethods);
}

const std::vector<std::string>::iterator LocationConfig::beginAcceptMethods() {
  return acceptMethods_.begin();
}

const std::vector<std::string>::iterator LocationConfig::endAcceptMethods() {
  return acceptMethods_.end();
}

const std::string LocationConfig::getElementAtIndexAcceptMethods(
    size_t index) const {
  if (index < acceptMethods_.size()) {
    return acceptMethods_[index];
  } else {
    throw std::invalid_argument("invalid index format");
  }
}

std::map<std::string, std::string> LocationConfig::getCgiPrograms() const {
  return cgiPrograms_;
}

void LocationConfig::setCgiPrograms(
    const std::map<std::string, std::string> &cgiPrograms) {
  cgiPrograms_ = cgiPrograms;
}

void LocationConfig::addCgiPrograms(
    const std::pair<std::string, std::string> &cgiPrograms) {
  cgiPrograms_.insert(cgiPrograms);
}

const std::map<std::string, std::string>::iterator
LocationConfig::beginCgiPrograms() {
  return cgiPrograms_.begin();
}

const std::map<std::string, std::string>::iterator
LocationConfig::endCgiPrograms() {
  return cgiPrograms_.end();
}

bool LocationConfig::hasCgiProgram(const std::string &key) const {
  return cgiPrograms_.find(key) != cgiPrograms_.end();
}

const std::string &LocationConfig::getCgiProgram(const std::string &key) const {
  return cgiPrograms_.at(key);
}

void LocationConfig::addErrorPage(int errorCode, const std::string &page) {
  errorPages_.insert(std::pair<int, std::string>(errorCode, page));
}

const std::string LocationConfig::getErrorPage(int errorCode) const {
  if (errorPages_.find(errorCode) == errorPages_.end()) {
    return "";
  }
  return errorPages_.at(errorCode);
}

const std::map<int, std::string> &LocationConfig::getErrorPages() const {
  return errorPages_;
}
