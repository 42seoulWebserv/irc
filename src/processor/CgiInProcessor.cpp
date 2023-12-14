#include "CgiInProcessor.hpp"

#include "CgiOutProcessor.hpp"
#include "ErrorPageProcessor.hpp"

CgiInProcessor::CgiInProcessor(ICgi& cgi, IClient& client)
    : client_(client), cgi_(cgi), isPushStr_(false) {
  cgi_.print(Log::info, "  CgiInProcessor");
}

ProcessResult CgiInProcessor::process() {
  if (isPushStr_ == false) {
    isPushStr_ = true;
    const char* request = client_.getRequest().getBody().c_str();
    cgi_.getWriteBuffer().push(request);
    cgi_.getWriteBuffer().markEOF();
  }
  if (cgi_.getWriteBuffer().isEOF() == true) {
    return ProcessResult().setNextProcessor(new CgiOutProcessor(cgi_, client_));
  }
  return ProcessResult();
}
