#ifndef IProcessor_HPP_
#define IProcessor_HPP_

#include "IClient.hpp"
#include "ProcessResult.hpp"

class IProcessor {
 public:
  virtual ~IProcessor(){};
  virtual ProcessResult process() = 0;
};

#endif
