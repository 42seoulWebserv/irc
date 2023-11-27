#include "MethodPostProcessor.hpp"

#include <fstream>
#include <iostream>

#include "ErrorPageProcessor.hpp"
#include "FilePath.hpp"

MethodPostProcessor::MethodPostProcessor(IClient &client)
    : client_(client), file_() {}

MethodPostProcessor::~MethodPostProcessor() {
  if (file_.is_open()) {
    file_.close();
  }
}

// 들어온 경로가 디렉토리라면 실패.
// 들어온 경로가 파일이라면 그 형태 그대로 생성.
ProcessResult MethodPostProcessor::process() {
  FilePath filepath = client_.getLocationConfig()->getRootPath();
  filepath.append(client_.getRequest().getUri());
  // 들어온값이 directory 형태라면 실패.
  if (filepath.isDirectory()) {
    std::cout << "error: POST: not allow form" << std::endl;
    client_.setResponseStatusCode(404);
    return ProcessResult().setNextProcessor(new ErrorPageProcessor(client_));
  }
  // 경로 존재 X
  FilePath directoryPath = FilePath::getDirectory(filepath);
  directoryPath = directoryPath.toDirectoryPath();
  if (!directoryPath.isExist()) {
    std::cout << "error: POST: non exist path" << std::endl;
    client_.setResponseStatusCode(404);
    return ProcessResult().setNextProcessor(new ErrorPageProcessor(client_));
  }
  // 들어온값에 이미 같은 이름의 파일이 존재한다면 실패.
  if (filepath.isFile()) {
    std::cout << "error: POST: Files that already exist" << std::endl;
    client_.setResponseStatusCode(404);
    return ProcessResult().setNextProcessor(new ErrorPageProcessor(client_));
  }
  std::string content = client_.getRequest().getBody();
  file_.open(filepath.c_str(), std::ios::binary | std::ios::trunc);
  if (file_.is_open() == false) {
    client_.setResponseStatusCode(502);
    return ProcessResult().setNextProcessor(new ErrorPageProcessor(client_));
  }
  file_ << content;
  if (file_.fail()) {
    client_.setResponseStatusCode(502);
    return ProcessResult().setNextProcessor(new ErrorPageProcessor(client_));
  }
  client_.setResponseStatusCode(201);
  client_.setResponseHeader("Content-Length", "0");
  client_.getDataStream().readStr(client_.getResponse().toString());
  client_.getDataStream().setEof(true);
  return ProcessResult().setWriteOn(true);
}
