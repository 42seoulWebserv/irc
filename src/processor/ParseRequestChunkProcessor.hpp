#ifndef ParseRequestChunkProcessor_HPP_
#define ParseRequestChunkProcessor_HPP_

#include "IProcessor.hpp"
#include "String.hpp"

class ParseRequestChunkProcessor : public IProcessor {
 public:
  enum READ_STATUS { SIZE_LINE, CHUNK, DONE };

  ParseRequestChunkProcessor(IClient &client);
  ProcessResult process();

 private:
  void setChunkLength();
  void parseChunk();
  void printParseBodyResult();

  enum READ_STATUS readStatus_;
  size_t chunkLength_;
  String chunk_;
  String sizeLine_;
  IClient &client_;
  Request request_;
};

#endif
