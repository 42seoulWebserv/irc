#include "UnsupportedMethodProcessor.hpp"

UnsupportedMethodProcessor::UnsupportedMethodProcessor(
    const RequestVO& request, const LocationConfig* config, int kq,
    IObserver<ResponseVO>* ob)
    : ob_(ob) {}
