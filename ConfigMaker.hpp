#ifndef CONFIGMAKER_HPP_
#define CONFIGMAKER_HPP_

#include "Config.hpp"
#include "Directive.hpp"

class ConfigMaker {
public:
  static RootConfig makeConfig(Directive directive);
};

#endif
