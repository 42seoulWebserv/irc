#ifndef FilePath_HPP_
#define FilePath_HPP_

#include <iostream>

class FilePath : public std::string {
 public:
  FilePath();
  FilePath(const std::string &path);

  enum accessType { READ, WRITE, EXECUTE };

  void setPath(const std::string &path);
  const std::string &getPath() const;
  void appendPath(const std::string &path);
  bool isExist() const;
  bool isDirectory() const;
  bool isFile() const;
  bool isAccessible(enum accessType type) const;
  long getFileSize() const;
  std::string toDirectoryPath();
  std::string toFilePath();

  static std::string getExtension(const std::string &path);
  static std::string getFileName(const std::string &path);
  static std::string getDirectory(const std::string &path);
};

#endif
