#include "CgiOutProcessor.hpp"

CgiOutProcessor::CgiOutProcessor(ICgi& cgi) : cgi_(cgi) {}

ProcessResult CgiOutProcessor::process() { return ProcessResult(); }
