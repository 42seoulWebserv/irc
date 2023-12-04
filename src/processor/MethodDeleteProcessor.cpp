#include "MethodDeleteProcessor.hpp"

#include "ErrorPageProcessor.hpp"
#include "FilePath.hpp"
#include "WaitProcessor.hpp"

MethodDeleteProcessor::MethodDeleteProcessor(IClient &client)
    : client_(client) {
  client_.print(Log::info, " MethodDeleteProcessor");
}

static void deleteFile(FilePath &filepath) {
  int code = remove(filepath.c_str());
  if (code != 0) {
    throw std::invalid_argument("file remove error");
  }
  Log::debug << " delete success" << filepath << NL;
}

ProcessResult MethodDeleteProcessor::process() {
  client_.print(Log::debug, " in delete method");
  FilePath filepath = client_.getRequestResourcePath();
  // 들어온값이 directory 형태라면 실패.
  if (filepath.isDirectory()) {
    client_.print(Log::debug, " DELETE: not allowed form");
    client_.setResponseStatusCode(404);
    return ProcessResult().setNextProcessor(new ErrorPageProcessor(client_));
  }
  FilePath directoryPath = FilePath::getDirectory(filepath);
  directoryPath = directoryPath.toDirectoryPath();
  if (!directoryPath.isExist()) {
    client_.print(Log::debug, " DELETE: Forbidden");
    client_.setResponseStatusCode(403);
    return ProcessResult().setNextProcessor(new ErrorPageProcessor(client_));
  }
  // 삭제할려는 파일이 존재하지않는다면 실패.
  if (!filepath.isFile()) {
    client_.print(Log::debug, " DELETE: non exits file");
    client_.setResponseStatusCode(204);  // 204 No Content
    return ProcessResult().setNextProcessor(new ErrorPageProcessor(client_));
  }
  deleteFile(filepath);
  client_.setResponseStatusCode(200);
  client_.setResponseHeader("Content-Length", "0");
  client_.getDataStream().readStr(client_.getResponse().toString());
  return ProcessResult().setWriteOn(true).setNextProcessor(new WaitProcessor());
}
