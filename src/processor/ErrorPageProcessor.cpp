#include "ErrorPageProcessor.hpp"

#include "CgiProcessor.hpp"
#include "MethodDeleteProcessor.hpp"
#include "MethodGetProcessor.hpp"
#include "MethodPostProcessor.hpp"

ErrorPageProcessor::ErrorPageProcessor(IClient& client) : client_(client) {}

ProcessResult ErrorPageProcessor::process() {
  ProcessResult res;
  return res;
}
