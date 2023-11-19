#ifndef IRequestProcessor_HPP_
#define IRequestProcessor_HPP_

#include "IClient.hpp"
#include "ProcessResult.hpp"

class IRequestProcessor {
 public:
  virtual ~IRequestProcessor(){};
  virtual ProcessResult process() = 0;
};

#endif
