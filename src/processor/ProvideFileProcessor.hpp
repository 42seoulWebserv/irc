#ifndef ProvideFileProcessor_HPP_
#define ProvideFileProcessor_HPP_

#include "FileEventController.hpp"
#include "FilePath.hpp"
#include "IProcessor.hpp"

class ProvideFileProcessor : public IProcessor,
                             public IObserver<FileEventController::Event> {
 public:
  ProvideFileProcessor(IClient &client, const FilePath &path);
  ~ProvideFileProcessor();
  ProcessResult process();
  void onEvent(const FileEventController::Event &e);

 private:
  IClient &client_;
  FilePath path_;
  FileEventController *file_;
  FileEventController::Event fileEvent_;
};

#endif
