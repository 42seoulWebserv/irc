#include "StringBuffer.hpp"

StringBuffer::StringBuffer() {}

void StringBuffer::addBuffer(const char* buffer) {
  buffer_ += std::string(buffer);
}

void StringBuffer::addBuffer(const std::vector<char>& buffer) {
  buffer_.insert(buffer_.size(), buffer.data(), buffer.size());
}

void StringBuffer::addBuffer(const std::string& buffer) { buffer_ += buffer; }

std::string StringBuffer::nextSeek(const std::string& needle) {
  int start = 0;
  if (cache_.find(needle) == cache_.end()) {
    start = cache_[needle] - needle.size();
    start = start > 0 ? start : 0;
  }
  size_t pos = buffer_.find(needle, start);
  if (pos == buffer_.npos) {
    cache_[needle] = buffer_.size();
    return "";
  }
  std::string foundStr = buffer_.substr(0, pos + needle.size());
  buffer_.erase(0, foundStr.size());

  std::vector<std::string> deleteList;
  for (std::map<std::string, int>::iterator it = cache_.begin();
       it != cache_.end(); it++) {
    int pos = cache_[it->first] - foundStr.size();
    if (pos <= 0) {
      deleteList.push_back(it->first);
    }
  }

  for (size_t i = 0; i < deleteList.size(); i++) {
    cache_.erase(deleteList[i]);
  }

  return foundStr;
}

std::string StringBuffer::nextBuffer(std::string::size_type size) {
  if (size > buffer_.size()) {
    return "";
  }
  std::string res = buffer_.substr(0, size);
  buffer_.erase(0, size);
  return res;
}

const std::string& StringBuffer::getBuffer() { return buffer_; }

size_t StringBuffer::size() { return buffer_.size(); }
