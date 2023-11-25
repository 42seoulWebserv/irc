#ifndef MethodPostProcessor_HPP_
#define MethodPostProcessor_HPP_

#include <fstream>

#include "IObserver.hpp"
#include "IRequestProcessor.hpp"
#include "LocationConfig.hpp"
#include "Request.hpp"
#include "Response.hpp"

class MethodPostProcessor : public IRequestProcessor {
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
