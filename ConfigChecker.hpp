#ifndef CONFIGCHECKER_HPP
#define CONFIGCHECKER_HPP

#include "BlockDirective.hpp"

class Checker {
public:
  Checker(BlockDirective directive);
  bool fail();
  BlockDirective getRootConfig();
};

#endif
