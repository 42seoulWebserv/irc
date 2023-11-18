#include "RequestProcessorFactory.hpp"

#include "MethodDeleteProcessor.hpp"
#include "MethodGetProcessor.hpp"
#include "MethodPostProcessor.hpp"
#include "UnsupportedMethodProcessor.hpp"

IRequestProcessor* RequestProcessorFactory::createRequestProcessor(
    const Request& request, const LocationConfig* config,
    IObserver<Response>* ob) {
  const std::vector<std::string>& accepts = config->getAcceptMethods();
  const std::string& method = request.getMethod();
  if (std::find(accepts.begin(), accepts.end(), method) == accepts.end()) {
    return new UnsupportedMethodProcessor(request, config, ob);
  }
  if (method == "GET") {
    return new MethodGetProcessor(request, config, ob);
  }
  if (method == "POST") {
    return new MethodPostProcessor(request, config, ob);
  }
  if (method == "DELETE") {
    return new MethodDeleteProcessor(request, config, ob);
  }
  return new UnsupportedMethodProcessor(request, config, ob);
}
