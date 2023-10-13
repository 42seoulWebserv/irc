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
  for (element = location.children_.begin(); element != location.children_.end();
       element++) {
    if (element->key_ == "return") {
      res.redirectionStatusCode_ = strToInteger(element->values_[0]);
      res.redirectionPath_ = element->values_[1];
    } else if (element->key_ == "accept_methods") {
      fillInAcceptMethods(res, element->values_);
    } else if (element->key_ == "cgi_extension") {
      fillInCgiExtension(res, element->values_);
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
    }
  }
  return res;
}

RootConfig ConfigMaker::makeConfig(Directive directive) {
  RootConfig res;
  std::vector<Directive>::iterator server;
  for (server = directive.children_.begin(); server != directive.children_.end();
       server++) {
    if (server->key_ == "server") {
      ServerConfig tmp = makeSingleServerConfig(*server);
      res.serverConfigs_.push_back(makeSingleServerConfig(*server));
    }
  }
  return res;
}
