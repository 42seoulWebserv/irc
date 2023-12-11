#ifndef ProvideFileProcessor_HPP_
#define ProvideFileProcessor_HPP_

#include <fstream>

#include "FilePath.hpp"
#include "IProcessor.hpp"

class ProvideFileProcessor : public IProcessor {
 public:
  ProvideFileProcessor(IClient &client, const FilePath &path);
  ~ProvideFileProcessor();
  ProcessResult process();
  ProcessResult processReadFile();

 private:
  IClient &client_;
  FilePath path_;
  int fileSize_;
  int totalReadSize_;
  bool processReadFile_;
  std::ifstream file_;
};

#endif
