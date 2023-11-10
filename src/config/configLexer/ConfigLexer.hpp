#ifndef CONFIGLEXER_HPP_
#define CONFIGLEXER_HPP_

#include <iostream>
#include <sstream>

#include "Directive.hpp"
#include "ParseResult.hpp"

class ConfigLexer {
 public:
  static const Directive run(const std::string rawContent);

 private:
  static const Directive parseResultToDirective(const ParseResult &res);
};

#endif
