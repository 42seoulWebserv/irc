#ifndef DIRECTIVE_HPP
#define DIRECTIVE_HPP

#include <iostream>
#include <map>
#include <vector>

class Directive {
public:
  Directive();
  Directive(const std::string &value);
  Directive(const Directive &src);
  Directive &operator=(const Directive &rhs);
  ~Directive();

  std::string key_;
  std::vector<std::string> values_;
  std::vector<Directive> children_;
};

#endif
