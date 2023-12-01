#include "ErrorPageProcessor.hpp"

#include <sstream>

#include "FilePath.hpp"
#include "MethodDeleteProcessor.hpp"
#include "MethodGetProcessor.hpp"
#include "MethodPostProcessor.hpp"
#include "ProvideFileProcessor.hpp"
#include "WaitProcessor.hpp"

ErrorPageProcessor::ErrorPageProcessor(IClient& client)
    : client_(client), onlyUseDefaultPage_(false) {
  std::stringstream ss;
  ss << " ErrorPageProcessor(" << client_.getResponse().getStatusCode() << ")";
  client_.print(Log::info, ss.str());
}

static std::string intToString(int number) {
  std::stringstream ss;
  ss << number;
  return ss.str();
}

ProcessResult ErrorPageProcessor::process() {
  const Response& response = client_.getResponse();
  const int code = response.getStatusCode();
  const std::string msg = response.getStatusMsg();

  const LocationConfig* config = client_.getLocationConfig();
  if (config != NULL) {
    FilePath errorPagePath = config->getErrorPage(code);
    FilePath absolutePath = config->getRootPath();
    absolutePath.appendPath(errorPagePath);
    if (onlyUseDefaultPage_ == false && errorPagePath.empty() == false &&
        absolutePath.isFile() && absolutePath.isAccessible(FilePath::READ)) {
      return ProcessResult().setNextProcessor(
          new ProvideFileProcessor(client_, absolutePath));
    }
  }
  std::stringstream ss;
  ss << "<html>" << CRLF;
  ss << "<head><title>" << code << " " << msg << "</title></head>" << CRLF;
  ss << "<body>" << CRLF;
  ss << "<center><h1>" << code << " " << msg << "</h1></center>" << CRLF;
  ss << "<hr><center>irc</center>" << CRLF;
  ss << "</body>" << CRLF;
  ss << "</html>" << CRLF;
  std::string body = ss.str();
  client_.setResponseHeader("Content-Length", intToString(body.size()));
  client_.setResponseHeader("Connection", "close");
  client_.getDataStream().readStr(response.toString() + body);
  client_.getDataStream().setEof(true);

  return ProcessResult().setWriteOn(true).setNextProcessor(new WaitProcessor());
}

void ErrorPageProcessor::forceProvideDefaultPage() {
  onlyUseDefaultPage_ = true;
}
