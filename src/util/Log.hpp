#ifndef Log_HPP_
#define Log_HPP_

#include <iostream>

#define LOG_DEBUG 4
#define LOG_INFO 3
#define LOG_WARN 2
#define LOG_ERROR 1

#ifndef LOG_LEVEL
#define LOG_LEVEL LOG_INFO
#endif

#define NL "\n"
#define LOG_RED "\033[31m"
#define LOG_GREEN "\033[32m"
#define LOG_YELLOW "\033[33m"
#define LOG_BLUE "\033[34m"
#define LOG_WHITE "\033[37m"
#define LOG_COLOR_RESET "\033[0m"

class Log : public std::ostream {
 public:
  static Log debug;
  static Log info;
  static Log warn;
  static Log error;

  template <typename T>
  Log& operator<<(const T& rhs) {
    if (level_ <= LOG_LEVEL) {
      std::cout << color_ << rhs << LOG_COLOR_RESET;
    }
    return *this;
  }

 private:
  Log(const char* color, int level);
  const char* color_;
  int level_;
};

#endif
