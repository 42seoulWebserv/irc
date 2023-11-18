#include "MethodPostProcessor.hpp"

MethodPostProcessor::MethodPostProcessor(const RequestVO& request,
                                         const LocationConfig* config,
                                         IObserver<ResponseVO>* ob)
    : ob_(ob) {
  (void)request;
  (void)config;
  (void)ob;
}
