#ifndef MethodDeleteProcessor_HPP_
#define MethodDeleteProcessor_HPP_

#include "IObserver.hpp"
#include "IRequestProcessor.hpp"
#include "LocationConfig.hpp"
#include "Request.hpp"
#include "Response.hpp"

class MethodDeleteProcessor : public IRequestProcessor {
 public:
  MethodDeleteProcessor(IClient &client);
  ProcessResult process();

 private:
  IClient &client_;
  Response response_;
};

#endif
