#include "CgiInProcessor.hpp"

#include "CgiEventController.hpp"
#include "CgiOutProcessor.hpp"
#include "ErrorPageProcessor.hpp"

CgiInProcessor::CgiInProcessor(ICgi &cgi)
    : cgi_(cgi), error_(false), cgiRequestEnd_(false) {}

ProcessResult CgiInProcessor::process() {
  std::cout << "in cgiInprocessor" << std::endl;
  return ProcessResult();
  // return ProcessResult().setNextProcessor(new CgiOutProcessor());
}
