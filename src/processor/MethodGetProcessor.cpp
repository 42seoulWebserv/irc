#include "MethodGetProcessor.hpp"

#include "ErrorPageProcessor.hpp"
#include "FilePath.hpp"

MethodGetProcessor::MethodGetProcessor(IClient& client)
    : client_(client), reader(NULL), response_(client_.getResponse()) {}

ProcessResult MethodGetProcessor::process() {
  FilePath filename = "." + client_.getLocationConfig()->getRootPath();
  filename.append(client_.getRequest().getUri());
  if (!filename.isExist()) {
    std::cout << "file not exist" << std::endl;
    return ProcessResult().setStatus(404).setNextProcessor(
        new ErrorPageProcessor(client_));
  }
  if (!filename.isFile()) {
    filename = FilePath(filename.toDirectoryPath());
    filename.append(client_.getLocationConfig()->getIndexPath());
  }
  if (!filename.isAccessible(FilePath::READ)) {
    std::cout << "file not accessible" << std::endl;
    return ProcessResult().setStatus(403).setNextProcessor(
        new ErrorPageProcessor(client_));
  }
  reader = FileReadEventController::addEventController(
      filename, this, &(client_.getDataStream()));
  return ProcessResult().setResponse(&response_);
}

void MethodGetProcessor::onEvent(const FileReadEventController::Event& p) {
  (void)p;
}
