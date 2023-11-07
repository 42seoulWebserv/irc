#ifndef IFileObserver_HPP_
#define IFileObserver_HPP_

#include <string>

#define FILE_SUCCESS 1
#define FILE_FAILURE 2
#define FILE_READ 4
#define FILE_WRITE 8

class IFileObserver {
public:

  virtual ~IFileObserver() {}
  virtual void fileEvent(int status, const std::string &result) = 0;

};

#endif
