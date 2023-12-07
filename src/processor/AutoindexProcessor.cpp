#include "AutoindexProcessor.hpp"

#include <dirent.h>
#include <sys/stat.h>

#include <iomanip>
#include <sstream>

#include "ErrorPageProcessor.hpp"
#include "FilePath.hpp"
#include "WaitProcessor.hpp"

AutoindexProcessor::AutoindexProcessor(IClient& client) : client_(client) {
  client_.print(Log::info, " AutoindexProcessor");
}

ProcessResult AutoindexProcessor::process() {
  FilePath uri = client_.getRequest().getUri();
  FilePath path = client_.getRequestResourcePath().toDirectoryPath();
  client_.print(Log::debug, "uri: " + uri);
  client_.print(Log::debug, "path: " + path);
  DIR* dir = opendir(path.c_str());
  if (dir == NULL) {
    client_.print(Log::error, std::string("GET: opendir failed"));
    client_.setResponseStatusCode(403);
    return ProcessResult().setNextProcessor(new ErrorPageProcessor(client_));
  }
  std::stringstream ss;
  ss << "<!DOCTYPE html>";
  ss << "<head><title>Index of " << uri << "</title></head>";
  ss << "<body>";
  ss << "<h1>Index of " << uri << "</h1>";
  ss << "<hr><pre>";
  dirent* entry;
  do {
    entry = readdir(dir);
    if (entry == NULL) {
      closedir(dir);
      break;
    }
    FilePath fileName(entry->d_name);
    if (fileName == ".") {
      continue;
    }
    if (entry->d_type == DT_DIR) {
      fileName = fileName.toDirectoryPath();
    }
    ss << "<a href=\"" << fileName << "\">" << fileName << "</a>";
    if (fileName == "../") {
      ss << "<br>";
    } else {
      ss << std::right << std::setw(70 - fileName.size())
         << getFileTimeInfo(fileName);
      ss << " ";
      ss << std::right << std::setw(10) << getFileSize(fileName);
      ss << "<br>";
    }
  } while (entry != NULL);
  ss << "</pre><hr></body></html>";
  std::stringstream tmp;
  tmp << ss.str().length();
  client_.setResponseStatusCode(200);
  client_.setResponseHeader("Content-Length", tmp.str());
  client_.getDataStream().push(client_.getResponse().toString());
  client_.getDataStream().push(ss.str());
  client_.getDataStream().markEOF();
  return ProcessResult().setWriteOn(true).setNextProcessor(new WaitProcessor());
}

const std::string AutoindexProcessor::getFileTimeInfo(FilePath fileName) {
  struct stat fileStat;
  stat(fileName.c_str(), &fileStat);
  char buff[20];
  struct tm* timeInfo = std::localtime(&fileStat.st_ctime);
  std::strftime(buff, sizeof(buff), "%d-%b-%y %H:%M", timeInfo);
  return std::string(buff);
}

const std::string AutoindexProcessor::getFileSize(FilePath fileName) {
  if (!fileName.isFile()) {
    return "-";
  }
  struct stat fileStat;
  stat(fileName.c_str(), &fileStat);
  std::stringstream ss;
  ss << fileStat.st_size;
  return ss.str();
}
