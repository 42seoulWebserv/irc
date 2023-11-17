#include <fstream>
#include <iostream>

#include "FilePath.hpp"
#include "FileWriteEventController.hpp"
#include "MethodPostProcessor.hpp"
#include "ResponseVO.hpp"

static void requestPrint(const RequestVO &request) {
  std::cout << "================= request =================" << std::endl;
  std::cout << "uri_: " << request.getUri() << std::endl;
  std::cout << "method_: " << request.getMethod() << std::endl;
  std::cout << "version_: " << request.getVersion() << std::endl;
  std::cout << "body_: " << request.getBody() << std::endl;
  // std::cout << "headers_: " << request.hasHeader("Content-Type") <<
  // std::endl; std::cout << "headers_: " << request.getHeader("Content-Type")
  // << std::endl; std::endl; std::map<std::string, std::string> headers_;
  std::cout << "================= ======= =================" << std::endl;
}

static void configPrint(const LocationConfig *config) {
  std::cout << "================= config =================" << std::endl;
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
  std::cout << "================= ====== =================" << std::endl;
}

static std::string removeSubstring(std::string &str,
                                   const std::string &substr) {
  std::string result = str;
  size_t pos = str.find(substr);
  if (pos != std::string::npos) {
    result.erase(pos, result.length());
  }
  return result;
}

// curl -X POST -d "key1=value1&key2=value2" http://localhost:8080/test/
// key1=value1&key2=value2
// <root>/<filename>.txt
static void writePostFile(FilePath fileName, std::string &body) {
  std::cout << "in application/x-www-form-urlencoded" << std::endl;
  std::cout << "fileName: " << fileName << std::endl;
  std::cout << "body: " << body << std::endl;

  std::string result = removeSubstring(fileName, ".html");
  result.erase(0, 1);
  result.append(".txt");
  std::cout << "result: " << result << std::endl;
  std::ofstream outputFile(result);
  if (!outputFile.is_open()) {
    std::cerr << "Error: POST: std::ofstream" << std::endl;
  }
  outputFile << body;
  outputFile.close();
}

// 해당 디렉토리 구조가 존재하고, accept_methods가 허용하는 경우에만 POST 동작
MethodPostProcessor::MethodPostProcessor(const RequestVO &request,
                                         const LocationConfig *config, int kq,
                                         IObserver<ResponseVO> *ob)
    : ob_(ob) {
  FilePath fileName = config->getRootPath();
  fileName.append(request.getUri());
  fileName.append(config->getIndexPath());

  std::string contentType = request.getHeader("Content-Type");
  std::string body = request.getBody();
  std::string content;

  // configPrint(config);
  // requestPrint(request);
  writePostFile(fileName, body);
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
