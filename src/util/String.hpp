#ifndef String_HPP_
#define String_HPP_

#include <string>
#include <vector>

class String : public std::string {
 public:
  String();
  String(const std::string &str);
  std::vector<std::string> split(const std::string &delim);
  String trim();
  String ltrim();
  String rtrim();
};

#endif