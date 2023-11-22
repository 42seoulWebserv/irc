#ifndef RedirectionProcessor_HPP_
#define RedirectionProcessor_HPP_

#include "IRequestProcessor.hpp"
#include "Response.hpp"

class RedirectionProcessor : public IRequestProcessor {
 public:
  RedirectionProcessor(IClient &client);
  ProcessResult process();

 private:
  IClient &client_;
  Response response_;
};

#endif
