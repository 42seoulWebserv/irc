#ifndef CONFIGMAKER_HPP_
#define CONFIGMAKER_HPP_

#include "Directive.hpp"
#include "RootConfig.hpp"
#include "ServerConfig.hpp"

class ConfigMaker {
 public:
  static RootConfig makeConfig(Directive directive);
};

#endif
