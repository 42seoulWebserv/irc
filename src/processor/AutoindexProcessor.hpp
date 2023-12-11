#ifndef AutoindexProcessor_HPP_
#define AutoindexProcessor_HPP_

#include "FilePath.hpp"
#include "IProcessor.hpp"

class AutoindexProcessor : public IProcessor {
 public:
  AutoindexProcessor(IClient &client);
  ProcessResult process();

 private:
  const std::string getFileTimeInfo(FilePath fileName);
  const std::string getFileSize(FilePath fileName);

  IClient &client_;
};

#endif
