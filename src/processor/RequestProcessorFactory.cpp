#include "RequestProcessorFactory.hpp"

#include "MethodGetProcessor.hpp"

IRequestProcessor* RequestProcessorFactory::createRequestProcessor(
    const RequestVO& request, const LocationConfig* config, int kq,
    IObserver<ResponseVO>* ob) {
  return new MethodGetProcessor(request, config, kq, ob);
}
