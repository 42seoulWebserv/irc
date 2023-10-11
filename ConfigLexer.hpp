#ifndef Lexer_HPP
#define Lexer_HPP

#include "Directive.hpp"
#include "parser/lexer/ParseResult.hpp"
#include <iostream>
#include <sstream>

class ConfigLexer {
public:
  static const Directive run(const std::string raw);

private:
  static const Directive parseResultToDirective(const ParseResult &res);
};

#endif
