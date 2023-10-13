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
    if (element->key == "return") {
      res.redirectionStatusCode = strToInteger(element->values[0]);
      res.redirectionPath = element->values[1];
    } else if (element->key == "accept_methods") {
      fillInAcceptMethods(res, element->values);
    } else if (element->key == "cgi_extension") {
      fillInCgiExtension(res, element->values);
    } else if (element->key == "index") {
      res.indexPath = element->values[0];
    }
  }
  return res;
}

ServerConfig makeSingleServerConfig(Directive server) {
  ServerConfig res;
  std::vector<Directive>::iterator element;
  for (element = server.children_.begin(); element != server.children_.end();
       element++) {
    if (element->key == "listen") {
      res.port = strToInteger(element->values[0]);
    } else if (element->key == "location") {
      res.locationConfigs.push_back(makeLocationConfig(*element));
    } else if (element->key == "server_name") {
      res.serverName = element->values[0];
    } else if (element->key == "client_max_content_size") {
      res.limitClientBodySize = strToInteger(element->values[0]);
    }
  }
  return res;
}

void printServerConfig(ServerConfig res) {
  std::cout << "listen: " << res.port << '\n';
  std::cout << "server_name: " << res.serverName << '\n';
  std::cout << "client_max_content_size: " << res.limitClientBodySize << 'm'
            << '\n';
  std::vector<LocationConfig>::iterator location;
  for (location = res.locationConfigs.begin();
       location != res.locationConfigs.end(); location++) {
    std::cout << "location " << location->uri << " {" << '\n';
    std::cout << "  return " << location->redirectionStatusCode << ' '
              << location->redirectionPath << '\n';
    std::cout << "  accept_methods ";
    std::vector<std::string>::iterator method;
    for (method = location->acceptMethods.begin();
         method != location->acceptMethods.end(); method++) {
      std::cout << *method << ' ';
    }
    std::cout << '\n';
    std::map<std::string, std::string>::iterator cgi;
    for (cgi = location->cgiPrograms.begin();
         cgi != location->cgiPrograms.end(); cgi++) {
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
