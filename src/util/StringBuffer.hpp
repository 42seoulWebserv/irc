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
  std::string nextBuffer(int size);
  const std::string &getBuffer();
  size_t size();

 private:
  int begin_;
  std::string buffer_;
  std::map<std::string, int> cache_;
};

#endif
