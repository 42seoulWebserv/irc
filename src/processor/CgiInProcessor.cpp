#include "CgiInProcessor.hpp"

#include "CgiEventController.hpp"
#include "CgiOutProcessor.hpp"
#include "ErrorPageProcessor.hpp"

CgiInProcessor::CgiInProcessor(ICgi &cgi)
    : cgi_(cgi), error_(false), cgiRequestEnd_(false) {}

ProcessResult CgiInProcessor::process() { return ProcessResult(); }
