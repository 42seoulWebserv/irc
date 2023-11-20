#include "MethodDeleteProcessor.hpp"

MethodDeleteProcessor::MethodDeleteProcessor(IClient &client)
    : client_(client) {}

ProcessResult MethodDeleteProcessor::process() { return ProcessResult(); }
