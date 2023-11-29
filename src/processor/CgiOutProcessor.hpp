#ifndef CgiOutProcessor_HPP_
#define CgiOutProcessor_HPP_

#include "CgiEventController.hpp"
#include "ICgi.hpp"
#include "IObserver.hpp"
#include "IProcessor.hpp"

class CgiOutProcessor : public IProcessor {
 public:
  CgiOutProcessor(ICgi &cgi);
  ProcessResult process();

 private:
  ICgi &cgi_;
};

#endif
