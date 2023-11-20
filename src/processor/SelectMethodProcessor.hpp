#ifndef SelectMethodProcessor_HPP_
#define SelectMethodProcessor_HPP_

#include "IRequestProcessor.hpp"
class SelectMethodProcessor : public IRequestProcessor {
 public:
  SelectMethodProcessor(IClient &client);
  ProcessResult process();

 private:
  IClient &client_;
};

#endif