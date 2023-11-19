#ifndef MethodPostProcessor_HPP_
#define MethodPostProcessor_HPP_

#include "IObserver.hpp"
#include "IRequestProcessor.hpp"
#include "LocationConfig.hpp"
#include "Request.hpp"
#include "Response.hpp"

class MethodPostProcessor : public IRequestProcessor {
 public:
  MethodPostProcessor(IClient &client);
  ProcessResult process();

 private:
  IClient &client_;
};

#endif
