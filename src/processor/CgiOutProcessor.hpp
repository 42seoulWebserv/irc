#ifndef CgiOutProcessor_HPP_
#define CgiOutProcessor_HPP_

#include "CgiEventController.hpp"
#include "IObserver.hpp"
#include "IRequestProcessor.hpp"

class CgiOutProcessor : public IRequestProcessor,
                        public IObserver<CgiEventController::Event> {
 public:
  CgiOutProcessor(IClient &client);
  ProcessResult process();
  void onEvent(const IObserver<CgiEventController::Event> &p);

 private:
  IClient &client_;
};

#endif
