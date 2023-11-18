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
  MethodGetProcessor(const Request &request, const LocationConfig *config,
                     IObserver<Response> *ob);
  void onEvent(const FileReadEventController::Event &event);

 private:
  IObserver<Response> *ob_;
};

#endif
