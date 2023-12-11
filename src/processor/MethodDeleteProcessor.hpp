#ifndef MethodDeleteProcessor_HPP_
#define MethodDeleteProcessor_HPP_

#include "IProcessor.hpp"

class MethodDeleteProcessor : public IProcessor {
 public:
  MethodDeleteProcessor(IClient &client);
  ProcessResult process();

 private:
  IClient &client_;
};

#endif
