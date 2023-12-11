#ifndef CgiInProcessor_HPP_
#define CgiInProcessor_HPP_

#include "ICgi.hpp"
#include "IProcessor.hpp"

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
