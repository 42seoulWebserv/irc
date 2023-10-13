#ifndef DIRECTIVE_HPP
#define DIRECTIVE_HPP

#include <iostream>
#include <map>
#include <vector>

class Directive {
public:
  Directive();
  Directive(const std::string &value);
  std::string key;
  std::vector<std::string> values;
  std::vector<Directive> children;
};

#endif
