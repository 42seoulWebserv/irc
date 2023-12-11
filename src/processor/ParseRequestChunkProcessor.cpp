#include "ParseRequestChunkProcessor.hpp"

#include <sstream>

#include "ErrorPageProcessor.hpp"
#include "SelectMethodProcessor.hpp"

ParseRequestChunkProcessor::ParseRequestChunkProcessor(IClient& client)
    : readStatus_(SIZE_LINE),
      chunkLength_(0),
      client_(client),
      request_(client.getRequest()) {
  client_.print(Log::info, " ParseRequestChunkProcessor");
}

ProcessResult ParseRequestChunkProcessor::process() {
  if (readStatus_ == SIZE_LINE && sizeLine_.empty()) {
    sizeLine_ = client_.getRecvBuffer().nextSeek("\r\n");
    if (!sizeLine_.empty()) {
      setChunkLength();
      if (chunkLength_ == 0) {
        readStatus_ = DONE;
      } else {
        readStatus_ = CHUNK;
      }
      sizeLine_.clear();
    }
  }
  if (readStatus_ == CHUNK && chunk_.empty()) {
    chunk_ = client_.getRecvBuffer().nextBuffer(chunkLength_ + 2);
    if (!chunk_.empty()) {
      chunk_.erase(chunk_.size() - 2);
      parseChunk();
      chunk_.clear();
      chunk_ = client_.getRecvBuffer().nextSeek("0\r\n");
      if (!chunk_.empty()) {
        if (chunk_ != "0\r\n") {
          chunk_.erase(chunk_.size() - 3);
          parseChunk();
        }
        readStatus_ = DONE;
      } else {
        readStatus_ = SIZE_LINE;
      }
    }
  }
  if (readStatus_ == DONE) {
    const LocationConfig* config = client_.getLocationConfig();
    if (!config) {
      client_.setResponseStatusCode(404);
      client_.setRequest(request_);
      return ProcessResult().setNextProcessor(new ErrorPageProcessor(client_));
    }
    if (config->getLimitClientBodySize() != 0 &&
        static_cast<long long>(request_.getBody().size()) >
            config->getLimitClientBodySize()) {
      client_.setResponseStatusCode(413);
      client_.setRequest(request_);
      return ProcessResult().setNextProcessor(new ErrorPageProcessor(client_));
    }
    client_.setRequest(request_);
    return ProcessResult().setNextProcessor(new SelectMethodProcessor(client_));
  }
  return ProcessResult();
}

void ParseRequestChunkProcessor::setChunkLength() {
  sizeLine_ = sizeLine_.trim();
  std::stringstream ss;
  ss << std::hex << sizeLine_;
  ss >> chunkLength_;
}

void ParseRequestChunkProcessor::parseChunk() {
  request_.setBody(request_.getBody() + chunk_);
}

void ParseRequestChunkProcessor::printParseBodyResult() {
  client_.print(Log::debug, " ========Body=========");
  client_.print(Log::debug, " " + chunk_ + "$");
  client_.print(Log::debug, " =====================");
}
