#ifndef StringBuffer_HPP_
#define StringBuffer_HPP_

#include <map>
#include <string>
#include <vector>

class StringBuffer {
 public:
  StringBuffer();
  void addBuffer(const char *buffer);
  void addBuffer(const std::vector<char> &buffer);
  void addBuffer(const std::string &buffer);
  std::string nextSeek(const std::string &needle);
  std::string nextBuffer(std::string::size_type size);
  const std::string &getBuffer();
  size_t size();

 private:
  std::string buffer_;
  std::map<std::string, int> cache_;
};

#endif
