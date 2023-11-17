#include "MethodGetProcessor.hpp"

MethodGetProcessor::MethodGetProcessor(const RequestVO& request,
                                       const LocationConfig* config, int kq,
                                       IObserver<ResponseVO>* ob)
    : ob_(ob) {
  FileReadEventController::addEventController(kq, "default.conf", this);
  (void)request;
  (void)config;
  (void)ob;
}

void MethodGetProcessor::onEvent(const FileReadEventController::Event& event) {
  ResponseVO response;
  if (event.type_ == FileReadEventController::FAIL) {
    response.setStatusCode(404);
  }
  if (event.type_ == FileReadEventController::SUCCESS) {
    response.setStatusCode(200);
  }
  ob_->onEvent(response);
}
