#ifndef RedirectionProcessor_HPP_
#define RedirectionProcessor_HPP_

#include "IProcessor.hpp"

class RedirectionProcessor : public IProcessor {
 public:
  RedirectionProcessor(IClient &client);
  ProcessResult process();

 private:
  IClient &client_;
};

#endif
