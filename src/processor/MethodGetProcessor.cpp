#include "MethodGetProcessor.hpp"

MethodGetProcessor::MethodGetProcessor(IClient& client)
    : client_(client), reader(NULL) {}

ProcessResult MethodGetProcessor::process() {
  reader = FileReadEventController::addEventController("default.conf", this);
  return ProcessResult();
}
