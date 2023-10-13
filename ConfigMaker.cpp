#include <cmath>

#include "ConfigMaker.hpp"
#include "ServerConfig.hpp"

int strToInteger(const std::string &s) {
  char *end = NULL;
  double tmp = std::strtod(s.c_str(), &end);
  int n = static_cast<int>(tmp);
  return n;
}

void fillInAcceptMethods(LocationConfig &res, std::vector<std::string> values) {
  std::vector<std::string>::iterator value;
  for (value = values.begin(); value != values.end(); value++) {
    res.acceptMethods_.push_back(*value);
  }
  return;
}

void fillInCgiExtension(LocationConfig &res, std::vector<std::string> values) {
  res.cgiPrograms_.insert(std::make_pair(values[0], values[1]));
  // res.cgiPrograms_.insert(values[0], values[1]);
  return;
}

LocationConfig makeLocationConfig(Directive location) {
  LocationConfig res;
  std::vector<Directive>::iterator element;
  res.uri_ = location.values_[0];
  for (element = location.children_.begin();
       element != location.children_.end(); element++) {
    if (element->key_ == "return") {
      res.redirectionStatusCode_ = strToInteger(element->values_[0]);
      res.redirectionPath_ = element->values_[1];
    } else if (element->key_ == "accept_methods") {
      fillInAcceptMethods(res, element->values_);
    } else if (element->key_ == "cgi_extension") {
      fillInCgiExtension(res, element->values_);
    } else if (element->key_ == "index") {
      res.indexPath_ = element->values_[0];
    }
  }
  return res;
}

ServerConfig makeSingleServerConfig(Directive server) {
  ServerConfig res;
  std::vector<Directive>::iterator element;
  for (element = server.children_.begin(); element != server.children_.end();
       element++) {
    if (element->key_ == "listen") {
      res.port_ = strToInteger(element->values_[0]);
    } else if (element->key_ == "location") {
      res.locationConfigs_.push_back(makeLocationConfig(*element));
    } else if (element->key_ == "server_name") {
      res.serverName_ = element->values_[0];
    } else if (element->key_ == "client_max_content_size") {
      res.limitClientBodySize_ = strToInteger(element->values_[0]);
    }
  }
  return res;
}

void printServerConfig(ServerConfig res) {
  std::cout << "listen: " << res.port_ << '\n';
  std::cout << "server_name: " << res.serverName_ << '\n';
  std::cout << "client_max_content_size: " << res.limitClientBodySize_ << 'm'
            << '\n';
  std::vector<LocationConfig>::iterator location;
  for (location = res.locationConfigs_.begin();
       location != res.locationConfigs_.end(); location++) {
    std::cout << "location " << location->uri_ << " {" << '\n';
    std::cout << "  return " << location->redirectionStatusCode_ << ' '
              << location->redirectionPath_ << '\n';
    std::cout << "  accept_methods ";
    std::vector<std::string>::iterator method;
    for (method = location->acceptMethods_.begin();
         method != location->acceptMethods_.end(); method++) {
      std::cout << *method << ' ';
    }
    std::cout << '\n';
    std::map<std::string, std::string>::iterator cgi;
    for (cgi = location->cgiPrograms_.begin();
         cgi != location->cgiPrograms_.end(); cgi++) {
      std::cout << "  cgi_extension " << cgi->first << ' ' << cgi->second
                << '\n';
    }
    std::cout << '}' << '\n';
  }
}

RootConfig ConfigMaker::makeConfig(Directive directive) {
  RootConfig res;
  std::vector<Directive>::iterator server;
  for (server = directive.children_.begin();
       server != directive.children_.end(); server++) {
    if (server->key_ == "server") {
      ServerConfig tmp = makeSingleServerConfig(*server);
      res.serverConfigs_.push_back(makeSingleServerConfig(*server));
    }
  }
  return res;
}
