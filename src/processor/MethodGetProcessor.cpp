#include "MethodGetProcessor.hpp"

#include <dirent.h>

#include <exception>
#include <sstream>

#include "ErrorPageProcessor.hpp"
#include "FilePath.hpp"
#include "ProvideFileProcessor.hpp"
#include "WaitProcessor.hpp"

MethodGetProcessor::MethodGetProcessor(IClient& client)
    : client_(client), response_(client_.getResponse()) {}

ProcessResult MethodGetProcessor::process() {
  FilePath path = client_.getLocationConfig()->getRootPath();
  path.append(client_.getRequest().getUri());
  if (!path.isExist()) {
    return ProcessResult().setStatus(404).setNextProcessor(
        new ErrorPageProcessor(client_));
  }
  if (!path.isFile() && path.isAccessible(FilePath::READ)) {
    path = FilePath(path.toDirectoryPath());
    FilePath indexPath = path + client_.getLocationConfig()->getIndexPath();
    if (!indexPath.isAccessible(FilePath::READ) &&
        client_.getLocationConfig()->getAutoindex()) {
      createAutoindex(path);
      return ProcessResult()
          .setResponse(&response_)
          .setWriteOn(true)
          .setNextProcessor(new WaitProcessor());
    } else {
      path = indexPath;
    }
  }
  if (!path.isAccessible(FilePath::READ)) {
    return ProcessResult().setStatus(403).setNextProcessor(
        new ErrorPageProcessor(client_));
  }
  response_.setStatusCode(200);
  return ProcessResult()
      .setResponse(&response_)
      .setNextProcessor(new ProvideFileProcessor(client_, path, response_));
}

void MethodGetProcessor::createAutoindex(FilePath path) {
  DIR* dir = opendir(path.c_str());
  try {
    if (dir == NULL) {
      throw std::invalid_argument("wrong path");
    }
  } catch (const std::exception& e) {
    std::cout << "Error: GET: " << e.what() << '\n';
    return;
  }
  std::string html =
      "<!DOCTYPE html><html lang =\"en\"><head><meta charset="
      "\"UTF-8\"><meta name=\"viewport\" content=\"width=device-width, "
      "initial-scale=1.0\"><title>Document</title></head><body>";
  html += "<header><h1>Index" + path + "</h1></header>";
  dirent* entry;
  do {
    entry = readdir(dir);
    if (entry == NULL) {
      closedir(dir);
      break;
    }
    FilePath filename(entry->d_name);
    if (entry->d_type == DT_DIR) {
      filename.toDirectoryPath();
    }
    std::string fullPath =
        "<a href=\"" + filename + "\">" + filename + "</a><br>";
    html += fullPath;
  } while (entry != NULL);
  html += "</body></html>";
  std::stringstream ss;
  ss << html.size();
  response_.setStatusCode(200);
  response_.setHeader("Content-Length", ss.str());
  client_.getDataStream().readStr(response_.toString());
  client_.getDataStream().readStr(html);
  client_.getDataStream().setEof(true);
}
