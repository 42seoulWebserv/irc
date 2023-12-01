#ifndef ParseRequestHeadProcessor_HPP_
#define ParseRequestHeadProcessor_HPP_

#include "IProcessor.hpp"
#include "String.hpp"

class ParseRequestHeadProcessor : public IProcessor {
 public:
  enum READ_STATUS { START_LINE, HEADER, BODY, DONE };

  ParseRequestHeadProcessor(IClient &client);
  ProcessResult process();

 private:
  void parseStartLine();
  void parseHeader();
  void parseHeaderLineByLine(String str);
  bool checkContentLength();
  bool isChunk();
  void printParseHeadResult();

  enum READ_STATUS readStatus_;
  String startLine_;
  String header_;
  IClient &client_;
  Request request_;
};

#endif
