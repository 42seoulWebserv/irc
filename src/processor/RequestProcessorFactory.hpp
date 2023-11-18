#ifndef RequestProcessorFactory_HPP_
#define RequestProcessorFactory_HPP_

#include "IObserver.hpp"
#include "IRequestProcessor.hpp"
#include "LocationConfig.hpp"
#include "Request.hpp"
#include "Response.hpp"

class RequestProcessorFactory {
 public:
  static IRequestProcessor *createRequestProcessor(const Request &request,
                                                   const LocationConfig *config,
                                                   IObserver<Response> *ob);
};

#endif
