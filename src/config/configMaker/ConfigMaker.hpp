#ifndef CONFIGMAKER_HPP_
#define CONFIGMAKER_HPP_

#include "ServerConfig.hpp"
#include "RootConfig.hpp"
#include "Directive.hpp"

class ConfigMaker {
public:
  static RootConfig makeConfig(Directive directive);
};

#endif
