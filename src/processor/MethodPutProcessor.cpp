#include "MethodPutProcessor.hpp"

#include <fstream>
#include <iostream>

#include "ErrorPageProcessor.hpp"
#include "FilePath.hpp"
#include "WaitProcessor.hpp"

MethodPutProcessor::MethodPutProcessor(IClient &client)
    : file_(), client_(client) {
  client_.print(Log::info, " MethodPutProcessor");
}

MethodPutProcessor::~MethodPutProcessor() {
  if (file_.is_open()) {
    file_.close();
  }
}

// 들어온 경로가 디렉토리라면 실패.
// 들어온 경로가 파일이라면 그 형태 그대로 생성.
ProcessResult MethodPutProcessor::process() {
  FilePath filepath = client_.getLocationConfig()->getRootPath();
  filepath.append(client_.getRequest().getUri());
  // 들어온값이 directory 형태라면 실패.
  if (filepath.isDirectory()) {
    client_.print(Log::debug, " PUT: not allowed form");
    client_.setResponseStatusCode(400);
    return ProcessResult().setNextProcessor(new ErrorPageProcessor(client_));
  }
  // 경로 존재 X
  FilePath directoryPath = FilePath::getDirectory(filepath);
  directoryPath = directoryPath.toDirectoryPath();
  if (!directoryPath.isExist()) {
    client_.print(Log::debug, " PUT: Forbidden");
    client_.setResponseStatusCode(403);
    return ProcessResult().setNextProcessor(new ErrorPageProcessor(client_));
  }
  bool existFile = filepath.isFile();
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
  client_.setResponseStatusCode(existFile ? 200 : 201);
  client_.setResponseHeader("Content-Length", "0");
  client_.getDataStream().readStr(client_.getResponse().toString());
  client_.getDataStream().setEof(true);
  return ProcessResult().setWriteOn(true).setNextProcessor(new WaitProcessor());
}
