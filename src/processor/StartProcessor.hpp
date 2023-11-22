#ifndef StartProcessor_HPP_
#define StartProcessor_HPP_

#include "IClient.hpp"
#include "IRequestProcessor.hpp"
#include "Response.hpp"

class StartProcessor : public IRequestProcessor {
 public:
  StartProcessor(IClient &client);
  ProcessResult process();

 private:
  IClient &client_;
  Response response_;
};

#endif