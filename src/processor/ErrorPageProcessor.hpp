#ifndef ErrorPageProcessor_HPP_
#define ErrorPageProcessor_HPP_

#define CRLF "\r\n"

#include "IRequestProcessor.hpp"
#include "Response.hpp"

class ErrorPageProcessor : public IRequestProcessor {
 public:
  ErrorPageProcessor(IClient &client);
  ProcessResult process();

 private:
  Response response_;
  IClient &client_;
};

#endif