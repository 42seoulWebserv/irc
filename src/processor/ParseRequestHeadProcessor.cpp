#include "ParseRequestHeadProcessor.hpp"

#include <exception>

#include "ErrorPageProcessor.hpp"
#include "ParseRequestBodyProcessor.hpp"
#include "ParseRequestChunkProcessor.hpp"
#include "String.hpp"
#include "StringBuffer.hpp"

ParseRequestHeadProcessor::ParseRequestHeadProcessor(IClient& client)
    : readStatus_(START_LINE), client_(client), request_(client.getRequest()) {}

ProcessResult ParseRequestHeadProcessor::process() {
  try {
    if (readStatus_ == START_LINE && startLine_.empty()) {
      startLine_ = client_.getRecvBuffer().nextSeek("\r\n");
      if (!startLine_.empty()) {
        parseStartLine();
      }
    }
    if (readStatus_ == HEADER && header_.empty()) {
      header_ = client_.getRecvBuffer().nextSeek("\r\n\r\n");
      if (!header_.empty()) {
        parseHeader();
      }
    }
    if (readStatus_ == BODY) {
      printParseHeadResult();  // debug
      client_.setRequest(request_);
      if (isChunk()) {
        return ProcessResult().setNextProcessor(
            new ParseRequestChunkProcessor(client_));
      }
      if (!checkContentLength()) {
        return ProcessResult().setNextProcessor(
            new ErrorPageProcessor(client_));
      }
      return ProcessResult().setNextProcessor(
          new ParseRequestBodyProcessor(client_));
    }
  } catch (std::length_error& e) {
    std::cout << "Error: " << e.what() << std::endl;
    client_.setResponseStatusCode(402);
    return ProcessResult().setNextProcessor(new ErrorPageProcessor(client_));
  } catch (std::exception& e) {
    std::cout << "Error: " << e.what() << std::endl;
    client_.setResponseStatusCode(401);
    return ProcessResult().setNextProcessor(new ErrorPageProcessor(client_));
  }
  return ProcessResult();
}

void ParseRequestHeadProcessor::parseStartLine() {
  StringBuffer buff;
  buff.addBuffer(startLine_.rtrim());
  String method = buff.nextSeek(" ");
  method = method.ltrim().rtrim();
  if (method.empty()) {
    throw std::invalid_argument('"' + startLine_ + "'" + " no space");
  } else if (method != "GET" && method != "POST" && method != "DELETE") {
    throw std::invalid_argument('"' + method + '"' + " wrong method");
  }
  request_.setMethod(method);
  String uri = buff.nextSeek(" ");
  uri = uri.ltrim().rtrim();
  if (uri.empty()) {
    throw std::invalid_argument(startLine_ + " no space");
  }
  request_.setUri(uri);
  String httpVersion = buff.getBuffer();
  httpVersion = httpVersion.ltrim().rtrim();
  if (httpVersion != "HTTP/1.1") {
    throw std::invalid_argument('"' + httpVersion + '"' +
                                " wrong HTTP version");
  }
  request_.setVersion(httpVersion);
  readStatus_ = HEADER;
}

void ParseRequestHeadProcessor::parseHeader() {
  std::vector<std::string> headers = header_.rtrim().split("\r\n");
  std::vector<std::string>::iterator it;
  for (it = headers.begin(); it != headers.end(); it++) {
    parseHeaderLineByLine(String(*it).ltrim().rtrim());
  }
  readStatus_ = BODY;
}

void ParseRequestHeadProcessor::parseHeaderLineByLine(String str) {
  StringBuffer buff;
  buff.addBuffer(str);
  std::string key = buff.nextSeek(":");
  if (key.empty()) {
    throw std::invalid_argument('"' + str + '"' + " no " + '"' + ':' + '"');
  } else if (key.find(' ') != std::string::npos) {
    throw std::invalid_argument('"' + str + '"' + " key has space");
  }
  key.erase(key.size() - 1);
  String value = buff.getBuffer();
  value = value.ltrim().rtrim();
  request_.setHeader(key, value);
}

bool ParseRequestHeadProcessor::checkContentLength() {
  std::map<std::string, std::string>::const_iterator it =
      request_.getHeaders().find("Content-Length");
  if (it != request_.getHeaders().end()) {
    char* end;
    double contentLen = std::strtod(it->second.c_str(), &end);
    if ((end && *end != '\0') || contentLen < 0) {
      client_.setResponseStatusCode(400);
      return false;
    }
    const LocationConfig* config = client_.getLocationConfig();
    if (!config) {
      client_.setResponseStatusCode(404);
      return false;
    }
    if (contentLen > config->getLimitClientBodySize()) {
      client_.setResponseStatusCode(413);
      return false;
    }
  }
  return true;
}

bool ParseRequestHeadProcessor::isChunk() {
  std::map<std::string, std::string>::const_iterator it =
      request_.getHeaders().find("Transfer-Encoding");
  if (it != request_.getHeaders().end()) {
    if (it->second == "chunked") {
      return true;
    }
  }
  return false;
}

void ParseRequestHeadProcessor::printParseHeadResult() {
  std::cout << "=====ParseResult=====\n";
  std::cout << "method: " << request_.getMethod() << "$" << std::endl;
  std::cout << "uri: " << request_.getUri() << "$" << std::endl;
  std::cout << "version: " << request_.getVersion() << "$" << std::endl;
  std::map<std::string, std::string>::const_iterator iter;
  for (iter = request_.getHeaders().begin();
       iter != request_.getHeaders().end(); iter++) {
    std::cout << iter->first << ": " << iter->second << "$" << std::endl;
  }
  std::cout << "=====================\n";
}
