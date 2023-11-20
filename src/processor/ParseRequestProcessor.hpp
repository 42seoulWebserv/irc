#ifndef ParseRequestProcessor_HPP_
#define ParseRequestProcessor_HPP_

#include "IRequestProcessor.hpp"

class ParseRequestProcessor : public IRequestProcessor {
 public:
  enum READ_STATUS { START_LINE, HEADER, BODY };

  ParseRequestProcessor(IClient &client);
  ProcessResult process();

 private:
  IClient &client_;

  void parseHeaderLineByLine(std::string str);
  void parseStartLine(std::string str);
  void printParseResult();
  void parseHeader();
  void parseBody();

  enum READ_STATUS readStatus_;
  size_t contentLength_;
  std::string headerBuffer_;
  std::string bodyBuffer_;
  Request request_;
};

#endif
