#include "SelectMethodProcessor.hpp"

#include <algorithm>

#include "CgiProcessor.hpp"
#include "ErrorPageProcessor.hpp"
#include "FilePath.hpp"
#include "MethodDeleteProcessor.hpp"
#include "MethodGetProcessor.hpp"
#include "MethodPostProcessor.hpp"
#include "RedirectionProcessor.hpp"

SelectMethodProcessor::SelectMethodProcessor(IClient& client)
    : client_(client) {
  client_.print(Log::info, " SelectMethodProcessor");
}

static bool cgiChecker(IClient& client_) {
  FilePath extension = FilePath::getExtension(client_.getRequest().getUri());
  if (!client_.getLocationConfig()->hasCgiProgram('.' + extension)) {
    return false;
  }
  return true;
}

ProcessResult SelectMethodProcessor::process() {
  ProcessResult res;
  if (client_.getLocationConfig() == NULL) {
    client_.setResponseStatusCode(503);
    return res.setNextProcessor(new ErrorPageProcessor(client_));
  }
  const std::string& method = client_.getRequest().getMethod();
  const std::vector<std::string>& accepts =
      client_.getLocationConfig()->getAcceptMethods();
  if (client_.getRequest().hasHeader("Connection")) {
    const std::string& conn = client_.getRequest().getHeader("Connection");
    client_.setResponseHeader("Connection", conn);
  }
  if (client_.getRequest().getVersion() != "HTTP/1.1") {
    client_.setResponseStatusCode(505);
    return res.setNextProcessor(new ErrorPageProcessor(client_));
  }
  if (cgiChecker(client_)) {
    return ProcessResult().setWriteOn(true).setNextProcessor(
        new CgiProcessor(client_));
  }
  if (client_.getLocationConfig()->getRedirectionStatusCode()) {
    int code = client_.getLocationConfig()->getRedirectionStatusCode();
    client_.setResponseStatusCode(code);
    return res.setNextProcessor(new RedirectionProcessor(client_));
  }
  if (std::find(accepts.begin(), accepts.end(), method) == accepts.end()) {
    client_.setResponseStatusCode(405);
    return res.setNextProcessor(new ErrorPageProcessor(client_));
  }
  if (method == "GET") {
    return res.setNextProcessor(new MethodGetProcessor(client_));
  }
  if (method == "POST") {
    return res.setNextProcessor(new MethodPostProcessor(client_));
  }
  if (method == "DELETE") {
    return res.setNextProcessor(new MethodDeleteProcessor(client_));
  }
  client_.setResponseStatusCode(500);
  return res.setNextProcessor(new ErrorPageProcessor(client_));
}

/*
curl -X GET -H "Content-Type: application/x-www-form-urlencoded" \
  -d "name=value" \
  "http://your-server/php-script.php"

cgi 조건
1. uri의 경로의 맨 마지막의 확장자를 기준으로 판별
client_.getRequest().getUri()의 rfind을 사용하여 확장자 확인

* 아래와 같은 상황으로 들어올때 어떤건 post이고 어떤건 cgi인지 어떻게 판별?
curl -X POST localhost:8080/cgi/test.cgi
curl -X POST localhost:8080/cgi/test.cgi
-> cgi을 우선적으로 생각. 하보고 없다면 생성.
-> post가 cgi에서 허용하는 확장자로 생성을 하고 post가 생성한 파일에 똑같이
입력하면 중복으로 확인하는 것이 아니라 cgi로 들어가는..?
혹시라도 된다면 실행권한 없애기
*/
