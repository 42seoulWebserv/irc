#include "MethodPostProcessor.hpp"

#include <fstream>
#include <iostream>

#include "ErrorPageProcessor.hpp"
#include "FilePath.hpp"

MethodPostProcessor::MethodPostProcessor(IClient &client)
    : client_(client), writer_(NULL), response_(client_.getResponse()) {}

MethodPostProcessor::~MethodPostProcessor() {
  if (writer_) {
    writer_->cancel();
  }
}

// 들어온 경로가 디렉토리라면 실패.
// 들어온 경로가 파일이라면 그 형태 그대로 생성.
ProcessResult MethodPostProcessor::process() {
  if (writer_) {
    return ProcessResult();
  }
  FilePath filepath = client_.getLocationConfig()->getRootPath();
  filepath.append(client_.getRequest().getUri());
  // 들어온값이 directory 형태라면 실패.
  if (filepath.isDirectory()) {
    std::cout << "error: POST: not allow form" << std::endl;
    return ProcessResult().setStatus(404).setNextProcessor(
        new ErrorPageProcessor(client_));
  }
  // 경로 존재 X
  FilePath directoryPath = FilePath::getDirectory(filepath);
  directoryPath = directoryPath.toDirectoryPath();
  if (!directoryPath.isExist()) {
    std::cout << "error: POST: non exist path" << std::endl;
    return ProcessResult().setStatus(404).setNextProcessor(
        new ErrorPageProcessor(client_));
  }
  // 들어온값에 이미 같은 이름의 파일이 존재한다면 실패.
  if (filepath.isFile()) {
    std::cout << "error: POST: Files that already exist" << std::endl;
    return ProcessResult().setStatus(404).setNextProcessor(
        new ErrorPageProcessor(client_));
  }
  std::string content = client_.getRequest().getBody();
  writer_ =
      FileWriteEventController::addEventController(filepath, content, this);
  response_.setStatusCode(201);
  client_.getDataStream().readStr(response_.toString());
  return ProcessResult().setResponse(&response_).setWriteOn(true);
}

void MethodPostProcessor::onEvent(const FileWriteEventController::Event &p) {
  client_.getDataStream().setEof(true);
}
