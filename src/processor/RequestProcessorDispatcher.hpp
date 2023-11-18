#ifndef RequestProcessorDispatcher_HPP_
#define RequestProcessorDispatcher_HPP_

#include "IObserver.hpp"
#include "IRequestProcessor.hpp"
#include "LocationConfig.hpp"
#include "Request.hpp"
#include "Response.hpp"

class RequestProcessorDispatcher {
 public:
  static IRequestProcessor *createRequestProcessor(const Request &request,
                                                   const LocationConfig *config,
                                                   IObserver<Response> *ob);
};

#endif
