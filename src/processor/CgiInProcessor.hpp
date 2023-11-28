#ifndef CgiInProcessor_HPP_
#define CgiInProcessor_HPP_

#include "IObserver.hpp"
#include "IRequestProcessor.hpp"
#include "LocationConfig.hpp"
#include "Request.hpp"
#include "Response.hpp"

class CgiInProcessor : public IRequestProcessor,
                       public IObserver<CgiEventController::Event> {
 public:
  CgiInProcessor(IClient &client);
  ProcessResult process();
  void onEvent(const IObserver<CgiEventController::Event> &p);

 private:
  IClient &client_;
  CgiEventController *cgi_;
  bool cgiRequestEnd_;
  bool error_;
};

#endif
