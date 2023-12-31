#include "ConfigChecker.hpp"

#include <cmath>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <limits>
#include <set>
#include <vector>

#include "Directive.hpp"
#include "FilePath.hpp"
#include "Log.hpp"
#include "Response.hpp"

// asset

static int strToInt(const std::string &s) {
  char *end = NULL;
  double tmp = std::strtod(s.c_str(), &end);
  int n = static_cast<int>(tmp);
  if ((end && std::strlen(end) != 1 && std::isprint(end[0])) ||
      std::isnan(tmp) || std::isinf(tmp) ||
      tmp < std::numeric_limits<int>::min() ||
      tmp > std::numeric_limits<int>::max()) {
    throw std::invalid_argument('"' + s + '"' + " port number is invalid");
  }
  return n;
}

// checkHTTP

static bool hasHttpDirective(Directive directive) {
  if (directive.getKey() == "http") {
    return true;
  }
  return false;
}

// checkServer -> listen

static void checkValidPort(int port) {
  if (port < 2 || port > 65536) {
    throw std::invalid_argument("port number is out of range");
  }
  return;
}

// checkServerLocation -> method

static void checkValidMethod(std::vector<std::string> values) {
  std::vector<std::string>::iterator value;
  for (value = values.begin(); value != values.end(); value++) {
    if (*value != "GET" && *value != "POST" && *value != "PUT" &&
        *value != "DELETE") {
      throw std::invalid_argument("invalid method");
    }
  }
  return;
}

// checkServerLocation -> cgi

static void checkValidCgiExtension(std::vector<std::string> values) {
  if (values[0].at(0) != '.') {
    throw std::invalid_argument("invalid cgi extension format");
  }
  return;
}

static void checkValidIndex(std::string str) {
  if (str.find(".") == std::string::npos) {
    throw std::invalid_argument("invalid index format");
  };
}

// checkServer -> location

static void checkRootDirective(Directive root) {
  if (root.getElementAtIndexValues(0).at(0) != '/') {
    throw std::invalid_argument("invalid root path");
  }
  return;
}

static void checkServerClientMaxBodySize(std::string str) {
  std::string errMsg = "client_max_body_size error";
  for (std::string::iterator i = str.begin(); i != str.end(); i++) {
    if (i < str.end() - 1 && !std::isdigit(*i)) {
      throw std::invalid_argument(errMsg);
    }
    if (i == str.end() - 1 && *i != 'm') {
      throw std::invalid_argument(errMsg);
    }
  }
  int clientMaxBodySize = strToInt(str);
  if (clientMaxBodySize > 1024) {
    throw std::invalid_argument(errMsg + ": over max size(1024m)");
  }
  return;
}

static void checkErrorPageDirective(const std::vector<std::string> &values) {
  for (size_t i = 0; i < values.size() - 1; i++) {
    int statusCode = strToInt(values[i]);
    if (Response::getStatusMsg(statusCode) == "") {
      throw std::invalid_argument("error_page status code is out of range");
    }
  }
  if (values.size()) {
    FilePath path = values.back();
    if (path.front() != '/' || path.back() == '/') {
      throw std::invalid_argument("invalid error page");
    }
  }
}

static void checkReturnStatusCode(std::string str) {
  int statusCode = strToInt(str);
  if (Response::getStatusMsg(statusCode) == "") {
    throw std::invalid_argument("return status code is out of range");
  }
  return;
}

static void checkAutoindex(std::string str) {
  if (str != "on" && str != "off") {
    throw std::invalid_argument("autoindex value should be [on / off]");
  }
  return;
}

static void checkServerLocation(Directive location) {
  std::vector<Directive>::iterator element;
  for (element = location.beginChildren(); element != location.endChildren();
       element++) {
    if (element->getKey() == "root") {
      checkRootDirective(*element);
    } else if (element->getKey() == "return") {
      checkReturnStatusCode(element->getElementAtIndexValues(0));
    } else if (element->getKey() == "index") {
      checkValidIndex(element->getElementAtIndexValues(0));
    } else if (element->getKey() == "client_max_body_size") {
      checkServerClientMaxBodySize(element->getElementAtIndexValues(0));
    } else if (element->getKey() == "error_page") {
      checkErrorPageDirective(element->getValues());
    } else if (element->getKey() == "autoindex") {
      checkAutoindex(element->getElementAtIndexValues(0));
    } else if (element->getKey() == "accept_methods") {
      checkValidMethod(element->getValues());
    } else if (element->getKey() == "cgi_extension") {
      checkValidCgiExtension(element->getValues());
    } else {
      throw std::invalid_argument('"' + element->getKey() + '"' +
                                  " is invalid server location directive");
    }
  }
  return;
}

static void checkServerName(std::string str) {
  for (std::string::iterator i = str.begin(); i != str.end(); i++) {
    if (!std::isalnum(*i) || *i == ';' || *i == ' ' || !std::isgraph(*i)) {
      throw std::invalid_argument("server_name error");
    }
  }
  return;
}

static void checkServerLocationDuplicate(std::string locationUri,
                                         std::set<std::string> &locationPaths) {
  if (locationUri.at(0) != '/' ||
      locationUri.at(locationUri.size() - 1) != '/') {
    throw std::invalid_argument('"' + locationUri + '"' +
                                " location path must be in absolute path form");
  }
  if (locationPaths.find(locationUri) == locationPaths.end()) {
    locationPaths.insert(locationUri);
  } else {
    throw std::invalid_argument('"' + locationUri + '"' +
                                " location path cannot be duplicated");
  }
}

static void checkServerDirective(Directive server) {
  std::set<std::string> locationPaths;
  std::set<std::string> serverNames;
  std::vector<Directive>::iterator element;
  bool isServerNameExist = false;
  bool isListenExist = false;
  for (element = server.beginChildren(); element != server.endChildren();
       element++) {
    if (element->getKey() == "root") {
      checkRootDirective(*element);
    } else if (element->getKey() == "listen") {
      if (isListenExist) {
        throw std::invalid_argument("listen already exist");
      }
      isListenExist = true;
      checkValidPort(strToInt(element->getElementAtIndexValues(0)));
    } else if (element->getKey() == "server_name") {
      if (isServerNameExist) {
        throw std::invalid_argument("server_name already exist");
      }
      isServerNameExist = true;
      checkServerName(element->getElementAtIndexValues(0));
    } else if (element->getKey() == "return") {
      checkReturnStatusCode(element->getElementAtIndexValues(0));
    } else if (element->getKey() == "index") {
      checkValidIndex(element->getElementAtIndexValues(0));
    } else if (element->getKey() == "client_max_body_size") {
      checkServerClientMaxBodySize(element->getElementAtIndexValues(0));
    } else if (element->getKey() == "error_page") {
      checkErrorPageDirective(element->getValues());
    } else if (element->getKey() == "autoindex") {
      checkAutoindex(element->getElementAtIndexValues(0));
    } else if (element->getKey() == "location") {
      checkServerLocationDuplicate(element->getElementAtIndexValues(0),
                                   locationPaths);
      checkServerLocation(*element);
    } else {
      throw std::invalid_argument("invalid server directive");
    }
  }
  return;
}

static void checkDirectiveChildren(Directive directive) {
  std::vector<Directive>::iterator it;
  int serverCnt = 0;
  for (it = directive.beginChildren(); it != directive.endChildren(); it++) {
    if (it->getKey() == "root") {
      checkRootDirective(*it);
    } else if (it->getKey() == "index") {
      checkValidIndex(it->getElementAtIndexValues(0));
    } else if (it->getKey() == "client_max_body_size") {
      checkServerClientMaxBodySize(it->getElementAtIndexValues(0));
    } else if (it->getKey() == "error_page") {
      checkErrorPageDirective(it->getValues());
    } else if (it->getKey() == "autoindex") {
      checkAutoindex(it->getElementAtIndexValues(0));
    } else if (it->getKey() == "server") {
      checkServerDirective(*it);
      serverCnt++;
    } else {
      throw std::invalid_argument('"' + it->getKey() + '"' +
                                  " is invalid config directive");
    }
  }
  if (serverCnt == 0) {
    throw std::invalid_argument("no server");
  }
}

void ConfigChecker::checkDirective(Directive directive) {
  try {
    if (!hasHttpDirective(directive)) {
      throw std::invalid_argument("http key invalid");
    }
    checkDirectiveChildren(directive);
  } catch (const std::exception &e) {
    Log::error << "Config: " << e.what() << NL;
    throw e;
  }
}
