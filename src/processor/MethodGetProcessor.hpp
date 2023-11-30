#ifndef MethodGetProcessor_HPP_
#define MethodGetProcessor_HPP_

#include "FilePath.hpp"
#include "IObserver.hpp"
#include "IProcessor.hpp"
#include "LocationConfig.hpp"
#include "Request.hpp"
#include "Response.hpp"

class MethodGetProcessor : public IProcessor {
 public:
  MethodGetProcessor(IClient &client);
  ProcessResult process();

 private:
  void createAutoindex(FilePath filename);

  IClient &client_;
};

#endif
