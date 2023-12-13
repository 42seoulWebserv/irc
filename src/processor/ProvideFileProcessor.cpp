#include "ProvideFileProcessor.hpp"

#include <sstream>

#include "ErrorPageProcessor.hpp"
#include "WaitProcessor.hpp"

ProvideFileProcessor::ProvideFileProcessor(IClient& client,
                                           const FilePath& path)
    : client_(client),
      path_(path),
      fileSize_(0),
      totalReadSize_(0),
      processReadFile_(false),
      file_() {
  client_.print(Log::info, " ProvideFileProcessor");
}

ProvideFileProcessor::~ProvideFileProcessor() {
  if (file_.is_open()) {
    file_.close();
  }
}

ProcessResult ProvideFileProcessor::process() {
  if (processReadFile_) {
    return processReadFile();
  }
  if (path_.isFile() == false || path_.isAccessible(FilePath::READ) == false) {
    client_.setResponseStatusCode(404);
    ErrorPageProcessor* errorPage = new ErrorPageProcessor(client_);
    errorPage->forceProvideDefaultPage();
    return ProcessResult().setNextProcessor(errorPage);
  }
  const Response& response = client_.getResponse();
  if (path_.getFileSize() == 0) {
    fileSize_ = 0;
    client_.setResponseHeader("Content-Length", "0");
    client_.getResponseStream().push(response.toString());
    client_.getResponseStream().markEOF();
    return ProcessResult().setWriteOn(true).setNextProcessor(
        new WaitProcessor());
  }

  fileSize_ = path_.getFileSize();
  std::stringstream ss;
  ss << path_.getFileSize();
  if (FilePath::getExtension(client_.getRequestResourcePath()) == "jpg") {
    client_.setResponseHeader("Content-Type", "image/jpeg");
  }
  client_.setResponseHeader("Content-Length", ss.str());
  client_.getResponseStream().push(response.toString());
  if (fileSize_ == 0) {
    client_.getResponseStream().markEOF();
    return ProcessResult().setWriteOn(true).setNextProcessor(
        new WaitProcessor());
  }
  file_.open(path_.c_str(), std::ios::binary);
  if (file_.is_open() == false) {
    return ProcessResult().setError(true);
  }
  processReadFile_ = true;
  return ProcessResult().setWriteOn(true);
}

ProcessResult ProvideFileProcessor::processReadFile() {
  int size = client_.getResponseStream().push(file_);
  if (size == DELAYED_FILE_READ) {
    return ProcessResult();
  }
  if (size == -1) {
    return ProcessResult().setError(true);
  }
  totalReadSize_ += size;

  if (size != 0) {
    return ProcessResult();
  }

  if (totalReadSize_ != fileSize_) {
    return ProcessResult().setError(true);
  }

  client_.getResponseStream().markEOF();
  return ProcessResult().setNextProcessor(new WaitProcessor());
}
