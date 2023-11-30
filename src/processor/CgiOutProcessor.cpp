#include "CgiOutProcessor.hpp"

#include <sstream>

#include "String.hpp"

CgiOutProcessor::CgiOutProcessor(ICgi& cgi, IClient& client)
    : cgi_(cgi), client_(client), isBody_(false), status_(200) {}

ProcessResult CgiOutProcessor::process() {
  // response body ㅈㅓ자ㅇ할 공공간간이 없없음음
  if (isBody_ == false) {
    String rawHeaders = cgi_.getRecvBuffer().nextSeek("\r\n\r\n");

    if (rawHeaders.empty() == false) {
      isBody_ = true;
      Response response = client_.getResponse();
      std::map<std::string, std::string> headers = parseHeader(rawHeaders);
      if (headers.empty()) {
        return ProcessResult().setError(true);
      }
      std::map<std::string, std::string>::const_iterator it;
      for (it = headers.begin(); it != headers.end(); it++) {
        client_.setResponseHeader(it->first, it->second);
      }
    }
  }
  return ProcessResult();
}

std::map<std::string, std::string> CgiOutProcessor::parseHeader(String& raw) {
  raw = raw.trim();
  Response response;
  std::map<std::string, std::string> res;
  std::vector<std::string> strHeaders = raw.split("\r\n");
  for (size_t i = 0; i < strHeaders.size(); i++) {
    std::string& header = strHeaders[i];
    size_t pos = header.find(':');
    if (pos == header.npos) {
      return res;
    }
    String key = String(header.substr(0, pos));
    String value = String(header.substr(pos + 1)).trim();
    // headers.insert(std::make_pair(key, value));
    if (key == "Status") {
      std::stringstream ss(value.substr(0, value.find(" ")));
      ss >> status_;
    } else {
      response.setHeader(key, value);
    }
  }
  if (response.hasHeader("Content-Type")) {
    res.clear();
    return res;
  }
}
