#include <cmath>
#include <cstdlib>

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
    res.addAcceptMethods(*value);
  }
  return;
}

void fillInCgiExtension(LocationConfig &res, std::vector<std::string> values) {
  res.addCgiPrograms(std::make_pair(values[0], values[1]));
  // res.cgiPrograms_.insert(std::make_pair(values[0], values[1]));
  return;
}

LocationConfig &makeLocationConfig(LocationConfig &res, Directive location) {
  std::vector<Directive>::iterator element;
  res.setUri(location.getElementAtIndexValues(0));
  for (element = location.beginChildren(); element != location.endChildren();
       element++) {
    if (element->getKey() == "root") {
      res.setRootPath(element->getElementAtIndexValues(0));
    } else if (element->getKey() == "client_max_content_size") {
      res.setLimitClientBodySize(strToInteger(element->getElementAtIndexValues(0)));
    } else if (element->getKey() == "return") {
      res.setRedirectionStatusCode(
          strToInteger(element->getElementAtIndexValues(0)));
      res.setRedirectionPath(element->getElementAtIndexValues(1));
    } else if (element->getKey() == "accept_methods") {
      fillInAcceptMethods(res, element->getValues());
    } else if (element->getKey() == "cgi_extension") {
      fillInCgiExtension(res, element->getValues());
    } else if (element->getKey() == "index") {
      res.setIndexPath(element->getElementAtIndexValues(0));
    }
  }
  return res;
}

ServerConfig &makeSingleServerConfig(ServerConfig &res, Directive server) {
  std::vector<Directive>::iterator element;
  for (element = server.beginChildren(); element != server.endChildren();
       element++) {
    if (element->getKey() == "root") {
      res.setRootPath(element->getElementAtIndexValues(0));
    } else if (element->getKey() == "client_max_content_size") {
      res.setLimitClientBodySize(strToInteger(element->getElementAtIndexValues(0)));
    } else if (element->getKey() == "listen") {
      res.setPort(strToInteger(element->getElementAtIndexValues(0)));
    } else if (element->getKey() == "location") {
      LocationConfig locationConf(res);
      res.addLocationConfigs(makeLocationConfig(locationConf, *element));
    } else if (element->getKey() == "server_name") {
      res.setServerName(element->getElementAtIndexValues(0));
    } else if (element->getKey() == "client_max_content_size") {
      res.setLimitClientBodySize(
          strToInteger(element->getElementAtIndexValues(0)));
    }
  }
  return res;
}

RootConfig ConfigMaker::makeConfig(Directive directive) {
  RootConfig res;
  std::vector<Directive>::iterator element;
  for (element = directive.beginChildren(); element != directive.endChildren();
       element++) {
    if (element->getKey() == "root") {
      res.setRootPath(element->getElementAtIndexValues(0));
    } else if (element->getKey() == "client_max_content_size") {
      res.setLimitClientBodySize(strToInteger(element->getElementAtIndexValues(0)));
    } else if (element->getKey() == "server") {
      ServerConfig serverConf(res);
      res.addServerConfigs(makeSingleServerConfig(serverConf, *element));
    }
  }
  return res;
}
