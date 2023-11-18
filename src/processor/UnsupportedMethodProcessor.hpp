#ifndef UnsupportedMethodProcessor_HPP_
#define UnsupportedMethodProcessor_HPP_

#include "IObserver.hpp"
#include "IRequestProcessor.hpp"
#include "LocationConfig.hpp"
#include "RequestVO.hpp"
#include "ResponseVO.hpp"

class UnsupportedMethodProcessor : public IRequestProcessor {
 public:
  UnsupportedMethodProcessor(const RequestVO &request,
                             const LocationConfig *config,
                             IObserver<ResponseVO> *ob);

 private:
  IObserver<ResponseVO> *ob_;
};

#endif
