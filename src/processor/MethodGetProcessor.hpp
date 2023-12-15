#ifndef MethodGetProcessor_HPP_
#define MethodGetProcessor_HPP_

#include "FilePath.hpp"
#include "IProcessor.hpp"

class MethodGetProcessor : public IProcessor {
 public:
  MethodGetProcessor(IClient &client);
  ProcessResult process();

 private:
  IClient &client_;
};

#endif
