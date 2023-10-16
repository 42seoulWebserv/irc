#ifndef PARSER_HPP_
#define PARSER_HPP_

#include <iostream>
#include <map>
#include <sstream>

#include "ParseResult.hpp"
#include "Pattern.hpp"

class Parser {
public:
  Parser();
  Parser(const Parser &rhs);
  Parser &operator=(const Parser &rhs);
  ~Parser();
  ParseResult compile(const std::string &root, const std::string &str);
  Parser &add(const Pattern &pattern);
  const Pattern &operator[](const std::string &name);

private:
  std::map<const std::string, const Pattern *> map_;
};

#endif
