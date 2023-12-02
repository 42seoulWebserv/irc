#ifndef MethodPutProcessor_HPP_
#define MethodPutProcessor_HPP_

#include <fstream>

#include "IObserver.hpp"
#include "IProcessor.hpp"
#include "LocationConfig.hpp"
#include "Request.hpp"
#include "Response.hpp"

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
