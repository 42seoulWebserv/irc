#ifndef ProvideFileProcessor_HPP_
#define ProvideFileProcessor_HPP_

#include "FilePath.hpp"
#include "FileReadEventController.hpp"
#include "IObserver.hpp"
#include "IRequestProcessor.hpp"
#include "Response.hpp"

class ProvideFileProcessor : public IRequestProcessor,
                             public IObserver<FileReadEventController::Event> {
 public:
  ProvideFileProcessor(IClient &client, const FilePath &path,
                       const Response &response);
  ~ProvideFileProcessor();
  ProcessResult process();
  void onEvent(const FileReadEventController::Event &e);

 private:
  IClient &client_;
  FilePath path_;
  Response response_;
  FileReadEventController *reader_;
  int fileSize_;
  bool fatalError_;
};

#endif
