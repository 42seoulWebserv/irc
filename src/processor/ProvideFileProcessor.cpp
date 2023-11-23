#include "ProvideFileProcessor.hpp"

#include <sstream>

#include "ErrorPageProcessor.hpp"
#include "FileReadEventController.hpp"
#include "WaitProcessor.hpp"

ProvideFileProcessor::ProvideFileProcessor(IClient& client,
                                           const FilePath& path)
    : client_(client),
      path_(path),
      reader_(NULL),
      fileSize_(0),
      fatalError_(false) {}

ProvideFileProcessor::~ProvideFileProcessor() {
  if (reader_) {
    reader_->cancel();
  }
}

ProcessResult ProvideFileProcessor::process() {
  if (fatalError_) {
    return ProcessResult().setError(true);
  }
  if (reader_ != NULL) {
    return ProcessResult();
  }
  if (path_.isFile() == false || path_.isAccessible(FilePath::READ) == false) {
    ErrorPageProcessor* errorPage = new ErrorPageProcessor(client_);
    errorPage->forceProvideDefaultPage();
    client_.setResponseStatusCode(404);
    return ProcessResult().setNextProcessor(errorPage);
  }
  const Response& response = client_.getResponse();
  if (path_.getFileSize() == 0) {
    fileSize_ = 0;
    client_.setResponseHeader("Content-Length", "0");
    client_.getDataStream().readStr(response.toString());
    client_.getDataStream().setEof(true);
    return ProcessResult().setWriteOn(true).setNextProcessor(
        new WaitProcessor());
  }
  fileSize_ = path_.getFileSize();
  std::stringstream ss;
  ss << path_.getFileSize();
  client_.setResponseHeader("Content-Length", ss.str());
  client_.getDataStream().readStr(response.toString());
  reader_ = FileReadEventController::addEventController(
      path_, this, &(client_.getDataStream()));
  return ProcessResult().setWriteOn(true);
}

void ProvideFileProcessor::onEvent(const FileReadEventController::Event& e) {
  reader_ = NULL;
  if (e.type_ == FileReadEventController::FAIL) {
    fatalError_ = true;
    return;
  }
  const int totalReadSize = client_.getDataStream().getTotalRead();
  const int responseHeaderSize = client_.getResponse().toString().size();
  const int bodySize = totalReadSize - responseHeaderSize;
  if (bodySize != fileSize_) {
    fatalError_ = true;
    return;
  }
  client_.getDataStream().setEof(true);
}
