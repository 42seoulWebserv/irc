#ifndef MethodPostProcessor_HPP_
#define MethodPostProcessor_HPP_

#include "FileEventController.hpp"
#include "FilePath.hpp"
#include "IProcessor.hpp"

class MethodPostProcessor : public IProcessor,
                            public IObserver<FileEventController::Event> {
 public:
  MethodPostProcessor(IClient &client);
  ~MethodPostProcessor();
  ProcessResult process();
  void onEvent(const FileEventController::Event &e);

 private:
  IClient &client_;
  FilePath path_;
  DataStream buffer_;
  FileEventController *file_;
  FileEventController::Event fileEvent_;
};

#endif
