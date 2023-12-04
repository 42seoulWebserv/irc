#ifndef StartProcessor_HPP_
#define StartProcessor_HPP_

#include "IClient.hpp"
#include "IProcessor.hpp"

class StartProcessor : public IProcessor {
 public:
  StartProcessor(IClient &client);
  ProcessResult process();

 private:
  IClient &client_;
};

#endif
