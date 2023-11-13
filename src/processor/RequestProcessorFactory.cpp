#include "RequestProcessorFactory.hpp"

#include "MethodDeleteProcessor.hpp"
#include "MethodGetProcessor.hpp"
#include "MethodPostProcessor.hpp"
#include "MethodPutProcessor.hpp"
#include "UnsupportedMethodProcessor.hpp"

IRequestProcessor* RequestProcessorFactory::createRequestProcessor(
    const RequestVO& request, const LocationConfig* config, int kq,
    IObserver<ResponseVO>* ob) {
  const std::vector<std::string>& accepts = config->getAcceptMethods();
  const std::string& method = request.getMethod();
  if (std::find(accepts.begin(), accepts.end(), method) == accepts.end()) {
    return new UnsupportedMethodProcessor(request, config, kq, ob);
  }
  if (method == "GET") {
    return new MethodGetProcessor(request, config, kq, ob);
  }
  if (method == "POST") {
    return new MethodPostProcessor(request, config, kq, ob);
  }
  if (method == "DELETE") {
    return new MethodDeleteProcessor(request, config, kq, ob);
  }
  if (method == "PUT") {
    return new MethodPutProcessor(request, config, kq, ob);
  }
  return new UnsupportedMethodProcessor(request, config, kq, ob);
}
