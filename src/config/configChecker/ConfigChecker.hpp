#ifndef CONFIGCHECKER_HPP
#define CONFIGCHECKER_HPP

#include "Directive.hpp"

class ConfigChecker {
 public:
  static void checkDirective(Directive directive);
  // static RootConfig &checkDirective(Directive directive);
  // Directive getRootConfig();
};

#endif
