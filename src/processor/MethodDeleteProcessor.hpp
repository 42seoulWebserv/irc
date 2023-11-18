#ifndef MethodDeleteProcessor_HPP_
#define MethodDeleteProcessor_HPP_

#include "IObserver.hpp"
#include "IRequestProcessor.hpp"
#include "LocationConfig.hpp"
#include "Request.hpp"
#include "Response.hpp"

class MethodDeleteProcessor : public IRequestProcessor {
 public:
  MethodDeleteProcessor(const Request &request, const LocationConfig *config,
                        IObserver<Response> *ob);

 private:
  IObserver<Response> *ob_;
};

#endif
