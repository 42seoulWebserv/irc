#include "MethodPostProcessor.hpp"

MethodPostProcessor::MethodPostProcessor(const Request& request,
                                         const LocationConfig* config,
                                         IObserver<Response>* ob)
    : ob_(ob) {
  (void)request;
  (void)config;
  (void)ob;
}
