#include "MethodPostProcessor.hpp"

MethodPostProcessor::MethodPostProcessor(IClient &client) : client_(client) {}

ProcessResult MethodPostProcessor::process() { return ProcessResult(); }
