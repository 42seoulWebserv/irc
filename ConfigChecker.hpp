#ifndef CONFIGCHECKER_HPP
#define CONFIGCHECKER_HPP

#include "Config.hpp"
#include "Directive.hpp"

class Checker {
public:
  static void checkDirective(Directive directive);
  // static RootConfig &checkDirective(Directive directive);
  Directive getRootConfig();
};

#endif
