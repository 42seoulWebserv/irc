#ifndef CgiInProcessor_HPP_
#define CgiInProcessor_HPP_

#include "ICgi.hpp"
#include "IObserver.hpp"
#include "IProcessor.hpp"
#include "LocationConfig.hpp"
#include "Request.hpp"
#include "Response.hpp"

class CgiInProcessor : public IProcessor {
 public:
  CgiInProcessor(ICgi &cgi, IClient &client);
  ProcessResult process();

 private:
  IClient &client_;
  ICgi &cgi_;
  bool isPushStr_;
};

#endif
