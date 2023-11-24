#include "FilePath.hpp"

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

FilePath::FilePath() {}

FilePath::FilePath(const std::string& path) { setPath(path); }

void FilePath::setPath(const std::string& path) { assign(path); }

const std::string& FilePath::getPath() const { return *this; }

void FilePath::appendPath(const std::string& path) {
  if (size() > 0 && back() != '/') {
    push_back('/');
  }
  if (path.size() > 0 && path[0] == '/') {
    append(path.substr(1));
    return;
  }
  append(path);
}

bool FilePath::isExist() const {
  struct stat statData;
  if (stat(c_str(), &statData)) {
    return false;
  }
  return true;
}

bool FilePath::isDirectory() const {
  struct stat statData;
  if (stat(c_str(), &statData)) {
    return false;
  }
  return S_ISDIR(statData.st_mode);
}

bool FilePath::isFile() const {
  struct stat statData;
  if (stat(c_str(), &statData)) {
    return false;
  }
  return S_ISREG(statData.st_mode);
}

bool FilePath::isAccessible(accessType type) const {
  if (type == READ) {
    return access(c_str(), R_OK) == 0;
  }
  if (type == WRITE) {
    return access(c_str(), W_OK) == 0;
  }
  if (type == EXECUTE) {
    return access(c_str(), X_OK) == 0;
  }
  return false;
}

long FilePath::getFileSize() const {
  struct stat statData;
  if (stat(c_str(), &statData)) {
    return -1;
  }
  return statData.st_size;
}

std::string FilePath::getExtension(const std::string& path) {
  size_t pos = path.rfind('.');
  if (pos == std::string::npos) {
    return "";
  }
  return path.substr(pos + 1);
}

std::string FilePath::toDirectoryPath() {
  if (back() != '/') {
    return *this + '/';
  }
  return *this;
}

std::string FilePath::toFilePath() {
  if (back() == '/') {
    return substr(0, size() - 1);
  }
  return *this;
}

const char& FilePath::front() const {
  if (size() == 0) {
    throw std::out_of_range("FilePath::front(): empty path");
  }
  return this->at(0);
}

const char& FilePath::back() const {
  if (size() == 0) {
    throw std::out_of_range("FilePath::back(): empty path");
  }
  return this->at(size() - 1);
}

std::string FilePath::getFileName(const std::string& path) {
  size_t pos = path.rfind('/');
  if (pos == std::string::npos) {
    return path;
  }
  return path.substr(pos + 1);
}

std::string FilePath::getDirectory(const std::string& path) {
  size_t pos = path.rfind('/');
  if (pos == std::string::npos) {
    return "";
  }
  return path.substr(0, pos);
}

std::ostream& operator<<(std::ostream& os, const FilePath& path) {
  os << path.getPath();
  return os;
}
