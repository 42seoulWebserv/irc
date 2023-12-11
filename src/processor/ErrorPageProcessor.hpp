#ifndef ErrorPageProcessor_HPP_
#define ErrorPageProcessor_HPP_

#define CRLF "\r\n"

#include "IProcessor.hpp"

class ErrorPageProcessor : public IProcessor {
 public:
  ErrorPageProcessor(IClient &client);
  ProcessResult process();
  void forceProvideDefaultPage();

 private:
  IClient &client_;
  bool onlyUseDefaultPage_;
};

#endif
