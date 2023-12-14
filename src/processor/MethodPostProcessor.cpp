#include "MethodPostProcessor.hpp"

#include "ErrorPageProcessor.hpp"
#include "FilePath.hpp"
#include "WaitProcessor.hpp"

MethodPostProcessor::MethodPostProcessor(IClient &client)
    : client_(client), path_(), file_(NULL), fileEvent_() {
  client_.print(Log::info, " MethodPostProcessor");
}

MethodPostProcessor::~MethodPostProcessor() {
  if (file_ != NULL) {
    file_->cancel();
  }
}

ProcessResult MethodPostProcessor::process() {
  if (fileEvent_.end_) {
    if (fileEvent_.error_) {
      client_.setResponseStatusCode(502);
      return ProcessResult().setNextProcessor(new ErrorPageProcessor(client_));
    }
    client_.setResponseStatusCode(201);
    client_.setResponseHeader("Content-Length", "0");
    client_.getResponseStream().push(client_.getResponse().toString());
    client_.getResponseStream().markEOF();
    return ProcessResult().setNextProcessor(new WaitProcessor());
  }

  if (file_ != NULL) {
    return ProcessResult();
  }

  FilePath filepath = client_.getRequestResourcePath();
  // 들어온값이 directory 형태라면 실패.
  if (filepath.isDirectory()) {
    client_.print(Log::debug, " POST: not allowed form");
    client_.setResponseStatusCode(400);
    return ProcessResult().setNextProcessor(new ErrorPageProcessor(client_));
  }
  // 경로 존재 X
  FilePath directoryPath = FilePath::getDirectory(filepath);
  directoryPath = directoryPath.toDirectoryPath();
  if (!directoryPath.isExist()) {
    client_.print(Log::debug, " POST: Forbidden");
    client_.setResponseStatusCode(403);
    return ProcessResult().setNextProcessor(new ErrorPageProcessor(client_));
  }
  // 들어온값에 이미 같은 이름의 파일이 존재한다면 실패.
  if (filepath.isFile()) {
    client_.print(Log::debug, " POST: Files that already exist");
    client_.setResponseStatusCode(409);
    return ProcessResult().setNextProcessor(new ErrorPageProcessor(client_));
  }
  buffer_.push(client_.getRequest().getBody());
  buffer_.markEOF();
  file_ = FileEventController::addEventController(filepath, buffer_, O_WRONLY,
                                                  this);
  if (file_ == NULL) {
    client_.setResponseStatusCode(502);
    return ProcessResult().setNextProcessor(new ErrorPageProcessor(client_));
  }
  return ProcessResult().setWriteOn(true);
}

void MethodPostProcessor::onEvent(const FileEventController::Event &e) {
  file_ = NULL;
  fileEvent_ = e;
}
