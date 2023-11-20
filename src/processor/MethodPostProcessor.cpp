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
// <root>/<filepath>.txt
static void writePostFile(FilePath &filepath, std::string &content) {
  std::ofstream outputFile(filepath + "post.txt");
  if (!outputFile.is_open()) {
    std::cerr << "Error: POST: std::ofstream" << std::endl;
  }
  outputFile << content;
  outputFile.close();
}

// 해당 디렉토리 구조가 존재하고, accept_methods가 허용하는 경우에만 POST 동작
// 생성할 file명 : post.txt
MethodPostProcessor::MethodPostProcessor(const RequestVO &request,
                                         const LocationConfig *config, int kq,
                                         IObserver<ResponseVO> *ob)
    : ob_(ob) {
  FilePath filepath = "." + config->getRootPath();
  filepath.append(request.getUri());
  filepath = filepath.toDirectoryPath();
  std::string content = request.getBody();
  if (!filepath.isExist()) { // error code
    std::cout << "directory exists: " << filepath << std::endl;
  } else {
    writePostFile(filepath, content);
  }
  FileWriteEventController::addEventController(kq, filepath, content, this);
  // configPrint(config);
  // requestPrint(request);
  (void)ob;
}

// 받은 내용들을 바탕으로 작성할 파트
// TODO : POST위치에 이미 파일이 있거나 -> 40* error code, POST를 허용하지않는
// uri인 경우 -> 저 까지는 X
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
