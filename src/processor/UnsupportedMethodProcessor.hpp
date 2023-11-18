#ifndef UnsupportedMethodProcessor_HPP_
#define UnsupportedMethodProcessor_HPP_

#include "IObserver.hpp"
#include "IRequestProcessor.hpp"
#include "LocationConfig.hpp"
#include "Request.hpp"
#include "Response.hpp"

class UnsupportedMethodProcessor : public IRequestProcessor {
 public:
  UnsupportedMethodProcessor(const Request &request,
                             const LocationConfig *config,
                             IObserver<Response> *ob);

 private:
  IObserver<Response> *ob_;
};

#endif
