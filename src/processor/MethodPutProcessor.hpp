#ifndef MethodPutProcessor_HPP_
#define MethodPutProcessor_HPP_

#include "FileEventController.hpp"
#include "FilePath.hpp"
#include "IProcessor.hpp"

class MethodPutProcessor : public IProcessor,
                           public IObserver<FileEventController::Event> {
 public:
  MethodPutProcessor(IClient &client);
  ~MethodPutProcessor();
  ProcessResult process();
  void onEvent(const FileEventController::Event &e);

 private:
  IClient &client_;
  FilePath path_;
  DataStream buffer_;
  FileEventController *file_;
  FileEventController::Event fileEvent_;
  bool existFile_;
};

#endif
