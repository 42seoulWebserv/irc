#ifndef RequestProcessorFactory_HPP_
#define RequestProcessorFactory_HPP_

#include "IObserver.hpp"
#include "IRequestProcessor.hpp"
#include "LocationConfig.hpp"
#include "RequestVO.hpp"
#include "ResponseVO.hpp"

class RequestProcessorFactory {
 public:
  static IRequestProcessor *createRequestProcessor(const RequestVO &request,
                                                   const LocationConfig *config,
                                                   IObserver<ResponseVO> *ob);
};

#endif
