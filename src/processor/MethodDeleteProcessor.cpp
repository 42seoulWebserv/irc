#include "MethodDeleteProcessor.hpp"

MethodDeleteProcessor::MethodDeleteProcessor(const RequestVO& request,
                                             const LocationConfig* config,
                                             int kq, IObserver<ResponseVO>* ob)
    : ob_(ob) {
  (void)request;
  (void)config;
  (void)kq;
  (void)ob;
}