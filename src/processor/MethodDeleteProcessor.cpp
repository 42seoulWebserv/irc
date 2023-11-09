#include "MethodDeleteProcessor.hpp"

MethodDeleteProcessor::MethodDeleteProcessor(const RequestVO& request,
                                             const LocationConfig* config,
                                             int kq, IObserver<ResponseVO>* ob)
    : ob_(ob) {}
