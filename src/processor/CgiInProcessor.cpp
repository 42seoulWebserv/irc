#include "CgiInProcessor.hpp"

#include "CgiOutProcessor.hpp"
#include "ErrorPageProcessor.hpp"

CgiInProcessor::CgiInProcessor(ICgi& cgi, IClient& client)
    : client_(client), cgi_(cgi), isPushStr_(false) {
  cgi_.print(Log::info, "  CgiInProcessor");
}

/*
이 함수에서 writeBuffer에 값을 채워넣는 역할.
flag를 사용하여 1번만 사용.
writeBuffer cgi에 다 밀어넣었다면 outProcessor 적용.
*/
ProcessResult CgiInProcessor::process() {
  if (isPushStr_ == false) {
    isPushStr_ = true;
    const char* request = client_.getRequest().getBody().c_str();
    cgi_.getWriteBuffer().readStr(request);
    cgi_.getWriteBuffer().setEof(true);
  }
  if (cgi_.getWriteBuffer().isEOF() == true) {
    return ProcessResult().setNextProcessor(new CgiOutProcessor(cgi_, client_));
  }
  return ProcessResult();
}
