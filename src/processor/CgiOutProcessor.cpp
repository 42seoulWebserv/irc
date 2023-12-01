#include "CgiOutProcessor.hpp"

#include <sstream>

#include "String.hpp"
#include "WaitProcessor.hpp"

CgiOutProcessor::CgiOutProcessor(ICgi& cgi, IClient& client)
    : cgi_(cgi), client_(client) {}

ProcessResult CgiOutProcessor::process() {
  String rawHeaders = cgi_.getRecvBuffer().nextSeek("\r\n\r\n");
  if (rawHeaders.empty()) {
    return ProcessResult();
  }
  Response response = client_.getResponse();
  std::map<std::string, std::string> headers = parseHeader(rawHeaders);
  if (headers.empty()) {
    return ProcessResult().setError(true);
  }
  std::map<std::string, std::string>::const_iterator it;
  for (it = headers.begin(); it != headers.end(); it++) {
    client_.setResponseHeader(it->first, it->second);
  }
  cgi_.end();
  return ProcessResult().setNextProcessor(new WaitProcessor());
}

std::map<std::string, std::string> CgiOutProcessor::parseHeader(String& raw) {
  raw = raw.trim();
  Response response;
  std::vector<std::string> strHeaders = raw.split("\r\n");
  int status = 200;
  for (size_t i = 0; i < strHeaders.size(); i++) {
    std::string& header = strHeaders[i];
    size_t pos = header.find(':');
    if (pos == header.npos) {
      return response.getHeaders();
    }
    String key = String(header.substr(0, pos));
    String value = String(header.substr(pos + 1)).trim();
    if (key == "Status") {
      std::stringstream ss(value.substr(0, value.find(" ")));
      ss >> status;
      client_.setResponseStatusCode(status);
    } else {
      response.setHeader(key, value);
    }
  }
  if (response.hasHeader("Content-Type") == false) {
    return std::map<std::string, std::string>();
  }
  return response.getHeaders();
}
