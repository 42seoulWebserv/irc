#include "String.hpp"

String::String() {}

String::String(const std::string& str) { assign(str); }

std::vector<std::string> String::split(const std::string& delim) {
  std::vector<std::string> result;
  if (delim.size() == 0) {
    result.push_back(*this);
    return result;
  }
  size_t prev = 0;
  while (prev < size()) {
    size_t pos = find(delim, prev);
    if (pos == std::string::npos) {
      result.push_back(substr(prev, pos));
      break;
    } else if (prev != pos) {
      result.push_back(substr(prev, pos - prev));
    }
    prev = pos + delim.size();
  }
  return result;
}

String String::trim() {
  size_t start = 0;
  size_t end = size();
  for (size_t i = 0; i < size(); i++) {
    if (std::isspace(at(i)) == false) {
      break;
    }
    start++;
  }
  for (size_t i = size() - 1; i >= 0; i--) {
    if (std::isspace(at(i)) == false) {
      break;
    }
    end--;
  }
  return String(substr(start, end - start));
}

String String::ltrim() {
  size_t start = 0;
  size_t end = size();
  for (size_t i = 0; i < size(); i++) {
    if (std::isspace(at(i)) == false) {
      break;
    }
    start++;
  }
  return String(substr(start, end - start));
}

String String::rtrim() {
  size_t start = 0;
  size_t end = size();
  for (size_t i = size() - 1; i >= 0; i--) {
    if (std::isspace(at(i)) == false) {
      break;
    }
    end--;
  }
  return String(substr(start, end - start));
}
