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
  CgiInProcessor(ICgi &cgi);
  ProcessResult process();

 private:
  ICgi &cgi_;
  bool cgiRequestEnd_;
  bool error_;
};

#endif
