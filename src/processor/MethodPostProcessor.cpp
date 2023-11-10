#include "MethodPostProcessor.hpp"

MethodPostProcessor::MethodPostProcessor(const RequestVO& request,
                                         const LocationConfig* config, int kq,
                                         IObserver<ResponseVO>* ob)
    : ob_(ob) {}
