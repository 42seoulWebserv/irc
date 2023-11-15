#include "MethodPostProcessor.hpp"

#include "FileWriteEventController.hpp"
#include "ResponseVO.hpp"

static void requestPrint(const RequestVO &request) {
  std::cout << "content: " << request.getUri() << std::endl;
  std::cout << "uri_: " << request.getUri() << std::endl;
  std::cout << "method_: " << request.getMethod() << std::endl;
  std::cout << "version_: " << request.getVersion() << std::endl;
  std::cout << "body_: " << request.getBody() << std::endl;
  std::cout << "headers_: " << request.hasHeader("Content-Type") << std::endl;
  std::cout << "headers_: " << request.getHeader("Content-Type") << std::endl;
  // std::map<std::string, std::string> headers_;
}

static void configPrint(const LocationConfig *config) {
  std::cout << "limitClientBodySize_: " << config->getLimitClientBodySize()
            << std::endl;
  std::cout << "autoIndex_: " << config->getAutoIndex() << std::endl;
  std::cout << "redirectionStatusCode_: " << config->getRedirectionStatusCode()
            << std::endl;
  std::cout << "uri_: " << config->getUri() << std::endl;
  std::cout << "rootPath_: " << config->getRootPath() << std::endl;
  std::cout << "indexPath_: " << config->getIndexPath() << std::endl;
  std::cout << "redirectionPath_: " << config->getRedirectionPath()
            << std::endl;
  // std::vector<std::string> acceptMethods_;
  // std::map<std::string, std::string> cgiPrograms_;
}

// curl -X POST -d "key1=value1&key2=value2" http://localhost:8080/test/
// curl -X POST -F "key1=value1" -F "key2=value2" http://localhost:8080/test/
MethodPostProcessor::MethodPostProcessor(const RequestVO &request,
                                         const LocationConfig *config, int kq,
                                         IObserver<ResponseVO> *ob)
    : ob_(ob) {
  // std::string filename = request.getUri();
  // std::string content = request.getBody();

  std::string filename;
  std::string content;

  // curl -X POST -d "key1=value1&key2=value2" http://localhost:8080/test/
  // "key1=value1
  // key2=value2"
  filename = config->getRootPath() + config->getUri() + config->getIndexPath();
  if (request.getHeader("Content-Type") ==
      "application/x-www-form-urlencoded") {

  }
  // curl -X POST -F "key1=value1" -F "key2=value2"  http://localhost:8080/test/
  else if (request.getHeader("Content-Type") == "multipart/form-data") {
  }
  // content = request.getBody();
  requestPrint(request);
  // configPrint(config);
  std::cout << "filename : " << filename << std::endl;
  // std::cout << "in MethodPostProcessor" << std::endl;
  FileWriteEventController::addEventController(kq, filename, content, this);
  (void)ob;
}

// 받은 내용들을 바탕으로 작성할 파트
// write는 왜 content_가 없는지.
void MethodPostProcessor::onEvent(
    const FileWriteEventController::Event &event) {
  ResponseVO response;
  response.setVersion("HTTP/1.1");

  if (event.type_ == FileWriteEventController::FAIL) {
    response.setStatusCode(404);
  }
  if (event.type_ == FileWriteEventController::SUCCESS) {
    response.setStatusCode(200);
  }
  // std::cout << "request : " << request.getBody() << std::endl;
  this->ob_->onEvent(response);
}
