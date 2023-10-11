#include <cctype>
#include <cmath>
#include <exception>
#include <iostream>
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
  if (directive.key == "http") {
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

// checkServer -> location

void checkServerLocation(Directive location) {
  std::vector<Directive>::iterator element;
  for (element = location.children.begin(); element != location.children.end();
       element++) {
    if (element->key == "return") {
      int statusCode = strToInt(element->values[0]);
      if (statusCode < 100 || statusCode > 600) {
        throw std::invalid_argument("status code is out of range");
      }
    } else if (element->key == "accept_methods") {
      checkValidMethod(element->values);
    } else if (element->key == "cgi_extension") {
      checkValidCgiExtension(element->values);
    }
  }
  return;
}

void checkSingleServerDirective(Directive server) {
  std::vector<Directive>::iterator element;
  for (element = server.children.begin(); element != server.children.end();
       element++) {
    if (element->key == "listen") {
      if (!isNumber(element->values[0])) {
        throw std::invalid_argument("listen port");
      }
      checkValidPort(strToInt(element->values[0]));
    } else if (element->key == "location") {
      checkServerLocation(*element);
    } else {
      throw std::invalid_argument("invalid directive");
    }
  }
  return;
}

void checkServerDirectives(Directive directive) {
  std::vector<Directive>::iterator server;
  for (server = directive.children.begin(); server != directive.children.end();
       server++) {
    if (server->key == "server") {
      checkSingleServerDirective(*server);
    }
  }
}

void Checker::checkDirective(Directive directive) {
  try {
    if (!hasHttpDirective(directive)) {
      throw std::invalid_argument("http key invalid");
    }
    checkServerDirectives(directive);
  } catch (const std::exception &e) {
    std::cerr << "Error: Config: " << e.what() << '\n';
  }
}

/*
우선 checker와 인스턴스화 하는 부분을 분리하고픔
1. 검사 먼저 하고 인스턴스화 하기
    - 인스턴스화 할 때 같은 내용을 또 읽어야 함

인스턴스화 먼저 하고, 검사한 뒤에, 안에 내용 집어넣기. 만약 문제가 생긴다면
인스턴스 지워짐
*/
