#include "MethodGetProcessor.hpp"

#include <dirent.h>

#include <exception>

#include "ErrorPageProcessor.hpp"
#include "FilePath.hpp"

void MethodGetProcessor::createAutoindex(FilePath filename) {
  std::string htmlHeader =
      "<!DOCTYPE html>< html lang =\"en\"><head><meta charset ="
      "\"UTF-8\"><meta name = \"viewport\" content=\"width=device-width, "
      "initial-scale=1.0\"><title>Document</title></head><body>";
  client_.getDataStream().readStr(htmlHeader);
  std::string header = "<header>Index" + filename + "</header>";
  client_.getDataStream().readStr(header);
  DIR* dir = opendir(filename.c_str());
  try {
    if (dir == NULL) {
      throw std::invalid_argument("wrong filename");
    }
  } catch (const std::exception& e) {
    std::cout << "Error: GET: " << e.what() << '\n';
    return;
  }
  dirent* entry;
  do {
    entry = readdir(dir);
    if (entry == NULL) {
      closedir(dir);
      return;
    }
    std::string path =
        "<a href=\"" + filename + entry->d_name + '>' + entry->d_name + "</a>";
    // std::cout << "filename: " << path << std::endl;
    client_.getDataStream().readStr(path);
  } while (entry != NULL);
  closedir(dir);
  std::string htmlFooter = "</body></html>";
  client_.getDataStream().readStr(htmlFooter);
  client_.getDataStream().setEof(true);
}

MethodGetProcessor::MethodGetProcessor(IClient& client)
    : client_(client),
      reader(NULL),
      response_(client_.getResponse()),
      complete_(false) {}

ProcessResult MethodGetProcessor::process() {
  if (complete_ || reader) {
    DataStream& data = client_.getDataStream();
    (void)data;
    return ProcessResult();
  }
  FilePath filename = "." + client_.getLocationConfig()->getRootPath();
  filename.append(client_.getRequest().getUri());
  if (!filename.isExist()) {
    std::cout << "file not exist" << std::endl;
    return ProcessResult().setStatus(404).setNextProcessor(
        new ErrorPageProcessor(client_));
  }
  if (!filename.isFile()) {
    filename = FilePath(filename.toDirectoryPath());
    if (client_.getLocationConfig()->getIndexPath() != "") {
      createAutoindex(filename);
      complete_ = true;
      return ProcessResult().setResponse(&response_).setWriteOn(true);
      // client_.getDataStream().readFile(1);
      // return ProcessResult().setWriteOn(true);
    } else {
      filename.append(client_.getLocationConfig()->getIndexPath());
    }
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
