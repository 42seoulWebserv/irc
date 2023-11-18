#include "MethodGetProcessor.hpp"

MethodGetProcessor::MethodGetProcessor(const Request& request,
                                       const LocationConfig* config,
                                       IObserver<Response>* ob)
    : ob_(ob) {
  FileReadEventController::addEventController("default.conf", this);
  (void)request;
  (void)config;
  (void)ob;
}

void MethodGetProcessor::onEvent(const FileReadEventController::Event& event) {
  Response response;
  if (event.type_ == FileReadEventController::FAIL) {
    response.setStatusCode(404);
  }
  if (event.type_ == FileReadEventController::SUCCESS) {
    response.setStatusCode(200);
  }
  ob_->onEvent(response);
}
