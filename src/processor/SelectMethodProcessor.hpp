#ifndef SelectMethodProcessor_HPP_
#define SelectMethodProcessor_HPP_

#include "IProcessor.hpp"
#include "Response.hpp"

class SelectMethodProcessor : public IProcessor {
 public:
  SelectMethodProcessor(IClient &client);
  ProcessResult process();

 private:
  IClient &client_;
};

#endif