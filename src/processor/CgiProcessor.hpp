#ifndef CgiProcessor_HPP_
#define CgiProcessor_HPP_

#include "IObserver.hpp"
#include "IProcessor.hpp"
#include "LocationConfig.hpp"
#include "Request.hpp"
#include "Response.hpp"

class CgiProcessor : public IProcessor {
 public:
  CgiProcessor(IClient &client);
  ProcessResult process();

 private:
  IClient &client_;
};

#endif
