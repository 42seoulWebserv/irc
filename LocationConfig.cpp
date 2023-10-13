#include "LocationConfig.hpp"

LocationConfig::LocationConfig(void) {}


LocationConfig::LocationConfig(const LocationConfig &src) { *this = src; }

LocationConfig &LocationConfig::operator=(const LocationConfig &rhs) {
  if (this == &rhs) return *this;
	this->uri_ = rhs.uri_;
	this->acceptMethods_ = rhs.acceptMethods_;
	this->rootPath_ = rhs.rootPath_;
	this->redirectionStatusCode_ = rhs.redirectionStatusCode_;
	this->redirectionPath_ = rhs.redirectionPath_;
	this->autoIndex_ = rhs.autoIndex_;
	this->indexFile_ = rhs.indexFile_;
	this->cgiPrograms_ = rhs.cgiPrograms_;
  return *this;
}

LocationConfig::~LocationConfig(void) {}

void LocationConfig::printLocationConfig(void) {
	std::cout << "location " << this->uri_ << " {" << '\n';
	std::cout << "  return " << this->redirectionStatusCode_ << ' '
						<< this->redirectionPath_ << '\n';
	std::cout << "  accept_methods ";
	std::vector<std::string>::iterator method;
	for (method = this->acceptMethods_.begin();
				method != this->acceptMethods_.end(); method++) {
		std::cout << *method << ' ';
	}
	std::cout << '\n';
	std::map<std::string, std::string>::iterator cgi;
	for (cgi = this->cgiPrograms_.begin();
				cgi != this->cgiPrograms_.end(); cgi++) {
		std::cout << "  cgi_extension " << cgi->first << ' ' << cgi->second
							<< '\n';
	}
	std::cout << '}' << '\n';
}
