#include "CgiInProcessor.hpp"

#include <unistd.h>

#include "CgiEventController.hpp"
#include "CgiOutProcessor.hpp"
#include "ErrorPageProcessor.hpp"

CgiInProcessor::CgiInProcessor(ICgi& cgi, IClient& client)
    : client_(client),
      cgi_(cgi),
      error_(false),
      cgiRequestEnd_(false),
      totalBytesSent_(0) {}

/*
fd에서 한번에 읽을 수 있는 content의 size가 정해져있으므로
body를 한번에 읽을 수 없는 경우를 생각해서 CgiInProcessor을 여러번 호출하여 모든
body를 읽어들이는것이 해당 Process의 목표.
*/
ProcessResult CgiInProcessor::process() {
  std::cout << "in CgiInProcessor" << std::endl;

  const char* request = client_.getRequest().getBody().c_str();
  const int size = client_.getRequest().getBody().size();
  if (totalBytesSent_ == size) {
    std::cout << "totalBytesSent_: " << totalBytesSent_ << std::endl;
    std::cout << "request: " << request << std::endl;
    return ProcessResult().setNextProcessor(new CgiOutProcessor(cgi_));
  } else if (totalBytesSent_ > size) {
    std::cerr << "cgi in fatal error" << std::endl;
    return ProcessResult().setError(true);
  }
  std::cout << "request: " << request << std::endl;
  int remain = size - totalBytesSent_;
  int requestSize = write(cgi_.getFd(), request + totalBytesSent_, remain);
  if (requestSize == -1) {
    perror("write");
    return ProcessResult().setError(true);
  }
  totalBytesSent_ += requestSize;
  return ProcessResult();
}
