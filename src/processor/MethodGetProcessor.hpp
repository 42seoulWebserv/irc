#ifndef MethodGetProcessor_HPP_
#define MethodGetProcessor_HPP_

#include "FilePath.hpp"
#include "FileReadEventController.hpp"
#include "IObserver.hpp"
#include "IRequestProcessor.hpp"
#include "LocationConfig.hpp"
#include "Request.hpp"
#include "Response.hpp"

class MethodGetProcessor : public IRequestProcessor,
                           IObserver<FileReadEventController::Event> {
 public:
  MethodGetProcessor(IClient &client);
  ProcessResult process();
  void onEvent(const FileReadEventController::Event &p);

 private:
  void createAutoindex(FilePath filename);

  bool complete_;
  IClient &client_;
  FileReadEventController *reader;
  Response response_;
};

#endif
