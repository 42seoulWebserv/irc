#include <iostream>

#include "Config.hpp"
#include "ConfigChecker.hpp"

RootConfig::RootConfig(void) {}

RootConfig::RootConfig(Directive Directive) {}

RootConfig::RootConfig(const RootConfig &src) { *this = src; }

RootConfig &RootConfig::operator=(const RootConfig &rhs) {
  if (this != &rhs) {
    this->serverConfigs = rhs.serverConfigs;
  }
  return *this;
}

RootConfig::~RootConfig(void) {}

void RootConfig::FillInConfig(void) {}

ServerConfig::ServerConfig() {}

LocationConfig::LocationConfig() {}
