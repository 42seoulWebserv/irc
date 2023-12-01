#ifndef ParseRequestBodyProcessor_HPP_
#define ParseRequestBodyProcessor_HPP_

#include "IProcessor.hpp"
#include "String.hpp"

class ParseRequestBodyProcessor : public IProcessor {
 public:
  enum READ_STATUS { START_LINE, HEADER, BODY, DONE };

  ParseRequestBodyProcessor(IClient &client);
  ProcessResult process();

 private:
  void setContentLength();
  void parseBody();
  void printParseBodyResult();

  enum READ_STATUS readStatus_;
  size_t contentLength_;
  String body_;
  IClient &client_;
  Request request_;
};

#endif
