#ifndef CgiOutProcessor_HPP_
#define CgiOutProcessor_HPP_

#include "CgiEventController.hpp"
#include "ICgi.hpp"
#include "IObserver.hpp"
#include "IProcessor.hpp"
#include "String.hpp"
#include "StringBuffer.hpp"

class CgiOutProcessor : public IProcessor {
 public:
  CgiOutProcessor(ICgi &cgi, IClient &client);
  ProcessResult process();

 private:
  ICgi &cgi_;
  IClient &client_;
  int isBody_;
  int status_;
  std::map<std::string, std::string> parseHeader(String &raw);
};

#endif
