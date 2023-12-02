#ifndef MethodPostProcessor_HPP_
#define MethodPostProcessor_HPP_

#include <fstream>

#include "IProcessor.hpp"

class MethodPostProcessor : public IProcessor {
 public:
  MethodPostProcessor(IClient &client);
  ~MethodPostProcessor();
  ProcessResult process();
  ProcessResult process(const ProcessResult &prevResult);

 private:
  std::ofstream file_;
  IClient &client_;
};

#endif
