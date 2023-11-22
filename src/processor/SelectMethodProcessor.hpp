#ifndef SelectMethodProcessor_HPP_
#define SelectMethodProcessor_HPP_

#include "IRequestProcessor.hpp"
#include "Response.hpp"

class SelectMethodProcessor : public IRequestProcessor {
 public:
  SelectMethodProcessor(IClient &client);
  ProcessResult process();

 private:
  IClient &client_;
  Response response_;
};

#endif