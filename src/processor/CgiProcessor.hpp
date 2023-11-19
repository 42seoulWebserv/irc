#ifndef CgiProcessor_HPP_
#define CgiProcessor_HPP_

#include "IObserver.hpp"
#include "IRequestProcessor.hpp"
#include "LocationConfig.hpp"
#include "Request.hpp"
#include "Response.hpp"

class CgiProcessor : public IRequestProcessor {
 public:
  CgiProcessor(IClient &client);
  ProcessResult process();

 private:
  IClient &client_;
};

#endif
