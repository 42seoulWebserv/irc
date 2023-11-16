#include "MethodPostProcessor.hpp"

#include "FilePath.hpp"
#include "FileWriteEventController.hpp"
#include "ResponseVO.hpp"

static void requestPrint(const RequestVO &request) {
  std::cout << "uri_: " << request.getUri() << std::endl;
  std::cout << "method_: " << request.getMethod() << std::endl;
  std::cout << "version_: " << request.getVersion() << std::endl;
  std::cout << "body_: " << request.getBody() << std::endl;
  // std::cout << "headers_: " << request.hasHeader("Content-Type") <<
  // std::endl; std::cout << "headers_: " << request.getHeader("Content-Type")
  // << std::endl; std::endl; std::map<std::string, std::string> headers_;
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
// key1=value1&key2=value2
static void applicationContentType() {
  std::cout << "in application/x-www-form-urlencoded" << std::endl;
}

// curl -X POST -F "key1=value1" -F "key2=value2"
// http://localhost:8080/test/
/*--------------------------5048a8abd04b6aab
  Content-Disposition: form-data; name="key1"

  value1
  --------------------------5048a8abd04b6aab
  Content-Disposition: form-data; name="key2"

  value2
  --------------------------5048a8abd04b6aab--*/
static void multipartContentType() {
  std::cout << "in multipart/form-data" << std::endl;
}

MethodPostProcessor::MethodPostProcessor(const RequestVO &request,
                                         const LocationConfig *config, int kq,
                                         IObserver<ResponseVO> *ob)
    : ob_(ob) {
  FilePath fileName = config->getRootPath();
  fileName.append(config->getUri());
  fileName.append(config->getIndexPath());
  std::cout << "fileName: " << fileName << std::endl;

  std::string content;
  std::string contentType = request.getHeader("Content-Type");
  std::cout << "!!!contentType: " << contentType << std::endl;

  if (contentType.find("application/x-www-form-urlencoded")) {
    applicationContentType();
  } else if (contentType.find("multipart/form-data;")) {
    multipartContentType();
  }
  content = request.getBody();
  // configPrint(config);
  requestPrint(request);
  FileWriteEventController::addEventController(kq, fileName, content, this);
  (void)ob;
}

// 받은 내용들을 바탕으로 작성할 파트
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
  this->ob_->onEvent(response);
}
