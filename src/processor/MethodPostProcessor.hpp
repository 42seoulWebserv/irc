#ifndef MethodPostProcessor_HPP_
#define MethodPostProcessor_HPP_

#include "IObserver.hpp"
#include "IRequestProcessor.hpp"
#include "LocationConfig.hpp"
#include "RequestVO.hpp"
#include "ResponseVO.hpp"

class MethodPostProcessor : public IRequestProcessor {
 public:
  MethodPostProcessor(const RequestVO &request, const LocationConfig *config,
                      int kq, IObserver<ResponseVO> *ob);

 private:
  IObserver<ResponseVO> *ob_;
};

#endif
