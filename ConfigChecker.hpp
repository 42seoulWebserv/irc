#ifndef CONFIGCHECKER_HPP
#define CONFIGCHECKER_HPP

#include "BlockDirective.hpp"
#include "Config.hpp"

class Checker {
public:
  static RootConfig &checkDirective(BlockDirective directive);
  BlockDirective getRootConfig();
};

#endif
