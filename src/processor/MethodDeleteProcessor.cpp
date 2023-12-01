#include "MethodDeleteProcessor.hpp"

#include <cstdio>

#include "ErrorPageProcessor.hpp"
#include "FilePath.hpp"
#include "WaitProcessor.hpp"

MethodDeleteProcessor::MethodDeleteProcessor(IClient &client)
    : client_(client) {}

static void deleteFile(FilePath &filepath) {
  int code = remove(filepath.c_str());
  if (code != 0) {
    throw std::invalid_argument("file remove error");
  }
  std::cout << "delete success" << filepath << std::endl;
}

ProcessResult MethodDeleteProcessor::process() {
  std::cout << "in delete method" << std::endl;
  FilePath filepath = client_.getLocationConfig()->getRootPath();
  filepath.append(client_.getRequest().getUri());
  // 들어온값이 directory 형태라면 실패.
  if (filepath.isDirectory()) {
    std::cout << "error: DELETE: not allowed form" << std::endl;
    client_.setResponseStatusCode(404);
    return ProcessResult().setNextProcessor(new ErrorPageProcessor(client_));
  }
  FilePath directoryPath = FilePath::getDirectory(filepath);
  directoryPath = directoryPath.toDirectoryPath();
  if (!directoryPath.isExist()) {
    std::cout << "error: DELETE: Forbidden" << std::endl;
    client_.setResponseStatusCode(403);
    return ProcessResult().setNextProcessor(new ErrorPageProcessor(client_));
  }
  // 삭제할려는 파일이 존재하지않는다면 실패.
  if (!filepath.isFile()) {
    std::cout << "error: DELETE: non exits file" << std::endl;
    client_.setResponseStatusCode(204);  // 204 No Content
    return ProcessResult().setNextProcessor(new ErrorPageProcessor(client_));
  }
  deleteFile(filepath);
  client_.setResponseStatusCode(200);
  client_.setResponseHeader("Content-Length", "0");
  client_.getDataStream().readStr(client_.getResponse().toString());
  return ProcessResult().setWriteOn(true).setNextProcessor(new WaitProcessor());
}
