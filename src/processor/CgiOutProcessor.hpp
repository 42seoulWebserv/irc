#ifndef CgiOutProcessor_HPP_
#define CgiOutProcessor_HPP_

#include "ICgi.hpp"
#include "IProcessor.hpp"
#include "String.hpp"

class CgiOutProcessor : public IProcessor {
 public:
  CgiOutProcessor(ICgi &cgi, IClient &client);
  ProcessResult process();

 private:
  ICgi &cgi_;
  IClient &client_;
  std::map<std::string, std::string> parseHeader(String &raw);
};

#endif
