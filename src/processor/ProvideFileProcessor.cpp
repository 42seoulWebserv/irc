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
  if (fileSize_ == 0) {
    client_.getDataStream().setEof(true);
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
  int size = client_.getDataStream().readFile(file_);
  totalReadSize_ += size;

  if (size == DELAYED_FILE_READ) {
    return ProcessResult();
  }
  if (size == -1) {
    return ProcessResult().setError(true);
  }
  if (size != 0) {
    return ProcessResult();
  }

  const int responseHeaderSize = client_.getResponse().toString().size();
  const int bodySize = totalReadSize_ - responseHeaderSize;
  if (bodySize != fileSize_) {
    return ProcessResult().setError(true);
  }

  client_.getDataStream().setEof(true);
  return ProcessResult().setNextProcessor(new WaitProcessor());
}
