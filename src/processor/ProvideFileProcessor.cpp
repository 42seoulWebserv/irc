#include "ProvideFileProcessor.hpp"

#include <sstream>

#include "ErrorPageProcessor.hpp"
#include "FileReadEventController.hpp"
#include "WaitProcessor.hpp"

ProvideFileProcessor::ProvideFileProcessor(IClient& client,
                                           const FilePath& path,
                                           const Response& response)
    : client_(client),
      path_(path),
      response_(response),
      reader_(NULL),
      fileSize_(0),
      fatalError_(false) {}

ProcessResult ProvideFileProcessor::process() {
  if (fatalError_) {
    return ProcessResult().setError(true);
  }
  if (reader_ != NULL) {
    return ProcessResult();
  }
  if (path_.isFile() == false && path_.isAccessible(FilePath::READ)) {
    ErrorPageProcessor* errorPage = new ErrorPageProcessor(client_);
    errorPage->forceProvideDefaultPage();
    return ProcessResult().setStatus(404).setNextProcessor(errorPage);
  }
  if (path_.getFileSize() == 0) {
    fileSize_ = 0;
    response_.setHeader("Content-Length", "0");
    client_.getDataStream().readStr(response_.toString());
    client_.getDataStream().setEof(true);
    return ProcessResult()
        .setWriteOn(true)
        .setResponse(&response_)
        .setNextProcessor(new WaitProcessor());
  }
  fileSize_ = path_.getFileSize();
  std::stringstream ss;
  ss << path_.getFileSize();
  response_.setHeader("Content-Length", ss.str());
  client_.getDataStream().readStr(response_.toString());
  reader_ = FileReadEventController::addEventController(
      path_, this, &(client_.getDataStream()));
  return ProcessResult().setWriteOn(true).setResponse(&response_);
}

void ProvideFileProcessor::onEvent(const FileReadEventController::Event& e) {
  if (e.type_ == FileReadEventController::FAIL) {
    fatalError_ = true;
    return;
  }
  const int totalReadSize = client_.getDataStream().getTotalRead();
  const int responseHeaderSize = response_.toString().size();
  const int bodySize = totalReadSize - responseHeaderSize;
  if (bodySize != fileSize_) {
    fatalError_ = true;
    return;
  }
  client_.getDataStream().setEof(true);
}
