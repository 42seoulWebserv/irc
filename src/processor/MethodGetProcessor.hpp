#ifndef MethodGetProcessor_HPP_
#define MethodGetProcessor_HPP_

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
  IClient &client_;
  FileReadEventController *reader;
  Response response_;
};

#endif
