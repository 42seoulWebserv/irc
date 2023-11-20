#include "CgiProcessor.hpp"

CgiProcessor::CgiProcessor(IClient &client) : client_(client) {}

struct ProcessResult CgiProcessor::process() { return ProcessResult(); }
