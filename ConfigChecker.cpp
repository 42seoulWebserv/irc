#include <cctype>
#include <cmath>
#include <exception>
#include <iostream>
#include <set>
#include <sstream>
#include <vector>

#include "ConfigChecker.hpp"
#include "Directive.hpp"

// asset

bool isNumber(const std::string &s) {
  std::string::const_iterator it = s.begin();
  while (it != s.end() && std::isdigit(*it))
    ++it;
  return !s.empty() && it == s.end();
}

int strToInt(const std::string &s) {
  char *end = NULL;
  double tmp = std::strtod(s.c_str(), &end);
  int n = static_cast<int>(tmp);
  if (std::isnan(tmp) || std::isinf(tmp) ||
      tmp < std::numeric_limits<int>::min() ||
      tmp > std::numeric_limits<int>::max()) {
    throw std::invalid_argument("port number is invalid");
  }
  return n;
}

// checkHTTP

bool hasHttpDirective(Directive directive) {
  if (directive.key_ == "http") {
    return true;
  }
  return false;
}

// checkServer -> listen

void checkValidPort(int port) {
  if (port < 0 || port > 65536) {
    throw std::invalid_argument("port number is out of range");
  }
  return;
}

// checkServerLocation -> method

void checkValidMethod(std::vector<std::string> values) {
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

void checkValidCgiExtension(std::vector<std::string> values) {
  if (values[0].at(0) != '.') {
    throw std::invalid_argument("invalid cgi extension format");
  }
  return;
}

void checkValidIndex(std::string str) {
  if (str.find(".") == std::string::npos) {
    throw std::invalid_argument("invalid index format");
  };
}

// checkServer -> location

void checkServerLocation(Directive location) {
  std::vector<Directive>::iterator element;
  for (element = location.children_.begin();
       element != location.children_.end(); element++) {
    if (element->key_ == "return") {
      int statusCode = strToInt(element->values_[0]);
      if (statusCode < 100 || statusCode > 600) {
        throw std::invalid_argument("status code is out of range");
      }
    } else if (element->key == "accept_methods") {
      checkValidMethod(element->values);
    } else if (element->key == "cgi_extension") {
      checkValidCgiExtension(element->values);
    } else if (element->key == "index") {
      checkValidIndex(element->values[0]);
    } else {
      throw std::invalid_argument("invalid server location directive");
    }
  }
  return;
}

void checkServerName(std::string str) {
  for (std::string::iterator i = str.begin(); i != str.end(); i++) {
    if (!std::isalnum(*i) || *i == ';' || *i == ' ' || !std::isgraph(*i)) {
      throw std::invalid_argument("server_name error");
    }
  }
  return;
}

void checkServerClientMaxContentSize(std::string str) {
  std::string errMsg = "client_max_content_size error";
  for (std::string::iterator i = str.begin(); i != str.end(); i++) {
    if (i < str.end() - 1 && !std::isdigit(*i)) {
      throw std::invalid_argument(errMsg);
    }
    if (i == str.end() - 1 && *i != 'm') {
      throw std::invalid_argument(errMsg);
    }
  }
  return;
}

void checkServerLocationDuplicate(std::string locationUri,
                                  std::set<std::string> &locationPaths) {
  if (locationPaths.find(locationUri) != locationPaths.end()) {
    locationPaths.insert(locationUri);
  } else {
    throw std::invalid_argument("location path cannot be duplicated");
  }
}

void checkServerDirective(Directive server) {
  std::set<std::string> locationPaths;
  std::set<std::string> serverNames;
  std::vector<Directive>::iterator element;
  bool isServerNameExist = false;
  for (element = server.children.begin(); element != server.children.end();
       element++) {
    if (element->key_ == "listen") {
      if (!isNumber(element->values_[0])) {
        throw std::invalid_argument("listen port");
      }
      checkValidPort(strToInt(element->values[0]));
    } else if (element->key == "location") {
      checkServerLocationDuplicate(element->values[0], locationPaths);
      checkServerLocation(*element);
    } else if (element->key == "server_name") {
      if (isServerNameExist) {
        throw std::invalid_argument("server_name already exist");
      }
      isServerNameExist = true;
      checkServerName(element->values[0]);
    } else if (element->key == "client_max_content_size") {
      checkServerClientMaxContentSize(element->values[0]);
    } else {
      throw std::invalid_argument("invalid server directive");
    }
  }
  return;
}

void checkRootDirective(Directive root) {
  if (root.values[0].at(0) != '/') {
    throw std::invalid_argument("invalid root path");
  }
  return;
}

void checkDirectives(Directive directive) {
  std::vector<Directive>::iterator it;
  for (it = directive.children.begin(); it != directive.children.end(); it++) {
    if (it->key == "server") {
      checkServerDirective(*it);
    } else if (it->key == "root") {
      checkRootDirective(*it);
    }
  }
}

void Checker::checkDirective(Directive directive) {
  try {
    if (!hasHttpDirective(directive)) {
      throw std::invalid_argument("http key invalid");
    }
    checkDirectives(directive);
  } catch (const std::exception &e) {
    std::cerr << "Error: Config: " << e.what() << '\n';
  }
}
