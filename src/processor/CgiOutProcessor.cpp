#include "CgiOutProcessor.hpp"

CgiOutProcessor::CgiOutProcessor(IClient& client) : client_(client) {}

ProcessResult CgiOutProcessor::process() { return ProcessResult(); }

void CgiOutProcessor::onEvent(const IObserver<CgiEventController::Event>& p) {
  (void)p;
}
