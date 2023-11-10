#include "MethodPutProcessor.hpp"

MethodPutProcessor::MethodPutProcessor(const RequestVO &request,
                                       const LocationConfig *config, int kq,
                                       IObserver<ResponseVO> *ob)
    : ob_(ob) {
  (void)request;
  (void)config;
  (void)kq;
  (void)ob;
  // conf 읽는 함수
  FileReadEventController::addEventController(kq, "default.conf", this);
}

// conf 쓰는 함수 (내용을 수정할 파트)
void MethodPutProcessor::onEvent(const FileReadEventController::Event &event,
                                 int kq) {
  // if (event.type_ == FileReadEventController::???) {
  //   ob_->onEvent(ResonseVO(404));
  //   return;
  // }
  // std::string content = request.getBody();
  std::string content;
  FileWriteEventController::addEventController(kq, "default.conf", content,
                                               this);
}

// 읽은 결과를 ResponseVO로 만드는 함수 (성공 실패 등등)
void MethodPutProcessor::onEvent(const FileWriteEventController::Event &event) {
  ResponseVO response;
  ob_->onEvent(response);
}
