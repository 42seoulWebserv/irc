#include "MethodGetProcessor.hpp"

MethodGetProcessor::MethodGetProcessor(IClient& client)
    : client_(client), reader(NULL) {}

ProcessResult MethodGetProcessor::process() {
  reader = FileReadEventController::addEventController(
      "default.conf", this, &(client_.getDataStream()));
  return ProcessResult();
}

void MethodGetProcessor::onEvent(const FileReadEventController::Event& p) {
  (void)p;
}
