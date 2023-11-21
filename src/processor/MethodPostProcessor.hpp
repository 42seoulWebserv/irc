#ifndef MethodPostProcessor_HPP_
#define MethodPostProcessor_HPP_

#include "FileWriteEventController.hpp"
#include "IObserver.hpp"
#include "IRequestProcessor.hpp"
#include "LocationConfig.hpp"
#include "Request.hpp"
#include "Response.hpp"

class MethodPostProcessor : public IRequestProcessor,
                            IObserver<FileWriteEventController::Event> {
 public:
  MethodPostProcessor(IClient &client);
  ProcessResult process();
  void onEvent(const FileWriteEventController::Event &p);

 private:
  IClient &client_;
  FileWriteEventController *writer_;
  Response response_;
};

#endif
