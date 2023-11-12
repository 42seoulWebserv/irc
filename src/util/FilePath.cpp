#include "FilePath.hpp"

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

FilePath::FilePath() {}

FilePath::FilePath(const std::string& path) { setPath(path); }

void FilePath::setPath(const std::string& path) { this->path_ = path; }

const std::string& FilePath::getPath() const { return path_; }

void FilePath::appendPath(const std::string& path) {
  if (path_.size() > 0 && path_.back() != '/') {
    path_.push_back('/');
  }
  if (path.size() > 0 && path[0] == '/') {
    path_.append(path.substr(1));
    return;
  }
  path_.append(path);
}

bool FilePath::isExist() const {
  struct stat stat_;
  if (stat(path_.c_str(), &stat_)) {
    return false;
  }
  return true;
}

bool FilePath::isDirectory() const {
  struct stat stat_;
  if (stat(path_.c_str(), &stat_)) {
    return false;
  }
  return S_ISDIR(stat_.st_mode);
}

bool FilePath::isFile() const {
  struct stat stat_;
  if (stat(path_.c_str(), &stat_)) {
    return false;
  }
  return S_ISREG(stat_.st_mode);
}

bool FilePath::isAccessible(accessType type) const {
  if (type == READ) {
    return access(path_.c_str(), R_OK) == 0;
  }
  if (type == WRITE) {
    return access(path_.c_str(), W_OK) == 0;
  }
  if (type == EXECUTE) {
    return access(path_.c_str(), X_OK) == 0;
  }
  return false;
}

std::string FilePath::getExtension(const FilePath& path) {
  return getExtension(path.getPath());
}

std::string FilePath::getExtension(const std::string& path) {
  size_t pos = path.rfind('.');
  if (pos == std::string::npos) {
    return "";
  }
  return path.substr(pos + 1);
}

std::string FilePath::toDirectoryPath() {
  if (path_.back() != '/') {
    return path_ + '/';
  }
  return path_;
}

std::string FilePath::toFilePath() {
  if (path_.back() == '/') {
    return path_.substr(0, path_.size() - 1);
  }
  return path_;
}

std::string FilePath::getFileName(const FilePath& path) {
  return getFileName(path.getPath());
}

std::string FilePath::getFileName(const std::string& path) {
  size_t pos = path.rfind('/');
  if (pos == std::string::npos) {
    return path;
  }
  return path.substr(pos + 1);
}

std::string FilePath::getDirectory(const FilePath& path) {
  return getDirectory(path.getPath());
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
