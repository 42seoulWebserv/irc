#ifndef MethodGetProcessor_HPP_
#define MethodGetProcessor_HPP_

#include "FilePath.hpp"
#include "IObserver.hpp"
#include "IRequestProcessor.hpp"
#include "LocationConfig.hpp"
#include "Request.hpp"
#include "Response.hpp"

class MethodGetProcessor : public IRequestProcessor {
 public:
  MethodGetProcessor(IClient &client);
  ProcessResult process();

 private:
  void createAutoindex(FilePath filename);

  IClient &client_;
};

#endif
