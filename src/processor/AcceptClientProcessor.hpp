#ifndef AccpetClientProcessor_HPP_
#define AccpetClientProcessor_HPP_

#include "IProcessor.hpp"
#include "IServer.hpp"

class AcceptClientProcessor : public IProcessor {
 public:
  AcceptClientProcessor(IServer &server);
  ProcessResult process();

 private:
  IServer &server_;
};

#endif