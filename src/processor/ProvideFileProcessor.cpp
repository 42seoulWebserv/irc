#include "ProvideFileProcessor.hpp"

#include <sstream>

#include "ErrorPageProcessor.hpp"
#include "WaitProcessor.hpp"

ProvideFileProcessor::ProvideFileProcessor(IClient& client,
                                           const FilePath& path)
    : client_(client), path_(path), file_(NULL), fileEvent_() {
  client_.print(Log::info, " ProvideFileProcessor");
}

ProvideFileProcessor::~ProvideFileProcessor() {
  if (file_ != NULL) {
    file_->cancel();
  }
}

ProcessResult ProvideFileProcessor::process() {
  if (fileEvent_.end_) {
    if (fileEvent_.error_) {
      return ProcessResult().setError(true);
    }
    return ProcessResult().setNextProcessor(new WaitProcessor());
  }

  if (file_ != NULL) {
    return ProcessResult();
  }

  if (path_.isFile() == false || path_.isAccessible(FilePath::READ) == false) {
    client_.setResponseStatusCode(404);
    ErrorPageProcessor* errorPage = new ErrorPageProcessor(client_);
    errorPage->forceProvideDefaultPage();
    return ProcessResult().setNextProcessor(errorPage);
  }

  const Response& response = client_.getResponse();
  long fileSize = path_.getFileSize();
  if (fileSize == 0) {
    client_.setResponseHeader("Content-Length", "0");
    client_.getResponseStream().push(response.toString());
    client_.getResponseStream().markEOF();
    return ProcessResult().setWriteOn(true).setNextProcessor(
        new WaitProcessor());
  }

  std::stringstream ss;
  ss << fileSize;
  if (FilePath::getExtension(client_.getRequestResourcePath()) == "jpg") {
    client_.setResponseHeader("Content-Type", "image/jpeg");
  }
  if (FilePath::getExtension(client_.getRequestResourcePath()) == "mp4") {
    client_.setResponseHeader("Content-Type", "video/mp4");
  }
  client_.setResponseHeader("Content-Length", ss.str());
  client_.getResponseStream().push(response.toString());

  file_ = FileEventController::addEventController(
      path_, client_.getResponseStream(), O_RDONLY, this);
  if (file_ == NULL) {
    client_.setResponseStatusCode(500);
    ErrorPageProcessor* errorPage = new ErrorPageProcessor(client_);
    errorPage->forceProvideDefaultPage();
    return ProcessResult().setNextProcessor(errorPage);
  }
  return ProcessResult().setWriteOn(true);
}

void ProvideFileProcessor::onEvent(const FileEventController::Event& e) {
  file_ = NULL;
  fileEvent_ = e;
  client_.getResponseStream().markEOF();
}
