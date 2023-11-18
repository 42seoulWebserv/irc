#ifndef MethodPutProcessor_HPP_
#define MethodPutProcessor_HPP_

#include "IObserver.hpp"
#include "IRequestProcessor.hpp"
#include "LocationConfig.hpp"
#include "RequestVO.hpp"
#include "ResponseVO.hpp"

class MethodPutProcessor : public IRequestProcessor {
 public:
  MethodPutProcessor(const RequestVO &request, const LocationConfig *config,
                     IObserver<ResponseVO> *ob);

 private:
  IObserver<ResponseVO> *ob_;
};

#endif
