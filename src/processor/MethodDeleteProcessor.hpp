#ifndef MethodDeleteProcessor_HPP_
#define MethodDeleteProcessor_HPP_

#include "IObserver.hpp"
#include "IProcessor.hpp"
#include "LocationConfig.hpp"
#include "Request.hpp"
#include "Response.hpp"

class MethodDeleteProcessor : public IProcessor {
 public:
  MethodDeleteProcessor(IClient &client);
  ProcessResult process();

 private:
  IClient &client_;
};

#endif
