#ifndef MethodPutProcessor_HPP_
#define MethodPutProcessor_HPP_

#include "IProcessor.hpp"

class MethodPutProcessor : public IProcessor {
 public:
  MethodPutProcessor(IClient &client);
  ~MethodPutProcessor();
  ProcessResult process();

 private:
  std::ofstream file_;
  IClient &client_;
};

#endif
