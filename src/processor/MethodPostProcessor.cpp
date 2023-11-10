#include "MethodPostProcessor.hpp"

#include "FileWriteEventController.hpp"
#include "ResponseVO.hpp"

MethodPostProcessor::MethodPostProcessor(const RequestVO &request,
                                         const LocationConfig *config, int kq,
                                         IObserver<ResponseVO> *ob)
    : ob_(ob) {

  std::string filename = request.getUri();
  std::string content = request.getBody();

  std::cout << "in MethodPostProcessor" << std::endl;
  FileWriteEventController::addEventController(kq, filename, content, this);
  (void)request;
  (void)config;
  (void)ob;
}

// 받은 내용들을 바탕으로 작성할 파트
void MethodPostProcessor::onEvent(
    const FileWriteEventController::Event &event) {
  ResponseVO response;

  this->ob_->onEvent(response);
}
