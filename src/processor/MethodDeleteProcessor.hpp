#ifndef MethodDeleteProcessor_HPP_
#define MethodDeleteProcessor_HPP_

#include "IObserver.hpp"
#include "IRequestProcessor.hpp"
#include "LocationConfig.hpp"
#include "RequestVO.hpp"
#include "ResponseVO.hpp"

class MethodDeleteProcessor : public IRequestProcessor {
 public:
  MethodDeleteProcessor(const RequestVO &request, const LocationConfig *config,
                        int kq, IObserver<ResponseVO> *ob);

 private:
  IObserver<ResponseVO> *ob_;
};

#endif
