#include "UnsupportedMethodProcessor.hpp"

UnsupportedMethodProcessor::UnsupportedMethodProcessor(IClient &client)
    : client_(client) {}

ProcessResult UnsupportedMethodProcessor::process() { return ProcessResult(); }
