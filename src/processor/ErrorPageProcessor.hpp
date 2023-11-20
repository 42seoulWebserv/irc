#ifndef ErrorPageProcessor_HPP_
#define ErrorPageProcessor_HPP_

#include "IRequestProcessor.hpp"
class ErrorPageProcessor : public IRequestProcessor {
 public:
  ErrorPageProcessor(IClient &client);
  ProcessResult process();

 private:
  IClient &client_;
};

#endif