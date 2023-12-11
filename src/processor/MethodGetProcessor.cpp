#include "MethodGetProcessor.hpp"

#include "AutoindexProcessor.hpp"
#include "ErrorPageProcessor.hpp"
#include "FilePath.hpp"
#include "ProvideFileProcessor.hpp"

MethodGetProcessor::MethodGetProcessor(IClient& client) : client_(client) {
  client_.print(Log::info, " MethodGetProcessor");
}

ProcessResult MethodGetProcessor::process() {
  FilePath path = client_.getRequestResourcePath();
  if (!path.isExist()) {
    client_.setResponseStatusCode(404);
    return ProcessResult().setNextProcessor(new ErrorPageProcessor(client_));
  }
  if (!path.isAccessible(FilePath::READ)) {
    client_.setResponseStatusCode(403);
    return ProcessResult().setNextProcessor(new ErrorPageProcessor(client_));
  }
  if (!path.isFile()) {
    path = FilePath(path.toDirectoryPath());
    FilePath indexPath = path + client_.getLocationConfig()->getIndexPath();
    if (!indexPath.isAccessible(FilePath::READ) &&
        client_.getLocationConfig()->getAutoindex()) {
      return ProcessResult().setNextProcessor(new AutoindexProcessor(client_));
    } else {
      path = indexPath;
    }
  }
  client_.setResponseStatusCode(200);
  return ProcessResult().setNextProcessor(
      new ProvideFileProcessor(client_, path));
}
