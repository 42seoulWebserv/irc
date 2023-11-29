#ifndef CgiProcessor_HPP_
#define CgiProcessor_HPP_

#include "CgiEventController.hpp"
#include "IClient.hpp"
#include "IObserver.hpp"
#include "IProcessor.hpp"

class CgiProcessor : public IProcessor,
                     public IObserver<CgiEventController::Event> {
 public:
  CgiProcessor(IClient &client);
  ~CgiProcessor();
  ProcessResult process();
  void onEvent(const CgiEventController::Event &e);

 private:
  IClient &client_;
  CgiEventController *cgi_;
  bool error_;
  bool end_;
};

#endif
