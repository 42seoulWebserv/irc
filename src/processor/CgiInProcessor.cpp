#include "CgiInProcessor.hpp"

#include <unistd.h>

#include "CgiEventController.hpp"
#include "CgiOutProcessor.hpp"
#include "ErrorPageProcessor.hpp"

CgiInProcessor::CgiInProcessor(ICgi& cgi, IClient& client)
    : client_(client), cgi_(cgi), error_(false), cgiRequestEnd_(false) {}

/*
fd에서 한번에 읽을 수 있는 content의 size가 정해져있으므로
body를 한번에 읽을 수 없는 경우를 생각해서 CgiInProcessor을 여러번 호출하여 모든
body를 읽어들이는것이 해당 Process의 목표.
*/
ProcessResult CgiInProcessor::process() {
  std::cout << "in CgiInProcessor" << std::endl;
  std::cout << "body: " << client_.getRequest().getBody() << std::endl;
  char* request = "get / http/1.1";  // cgiInProcessor에서 보내줄 내용들
  int requestSize = write(cgi_.getFd(), request, std::strlen(request));
  perror("write");
  std::cerr << "parent write size: " << requestSize << std::endl;
  if (cgiRequestEnd_) {
    return ProcessResult().setNextProcessor(new CgiOutProcessor(cgi_));
  }
  return ProcessResult();
}
