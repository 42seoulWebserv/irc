#ifndef MethodPostProcessor_HPP_
#define MethodPostProcessor_HPP_

#include "IObserver.hpp"
#include "IRequestProcessor.hpp"
#include "LocationConfig.hpp"
#include "Request.hpp"
#include "Response.hpp"

class MethodPostProcessor : public IRequestProcessor {
 public:
  MethodPostProcessor(const Request &request, const LocationConfig *config,
                      IObserver<Response> *ob);

 private:
  IObserver<Response> *ob_;
};

#endif
