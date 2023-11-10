#include "MethodPutProcessor.hpp"

MethodPutProcessor::MethodPutProcessor(const RequestVO& request,
                                       const LocationConfig* config, int kq,
                                       IObserver<ResponseVO>* ob)
    : ob_(ob) {}
