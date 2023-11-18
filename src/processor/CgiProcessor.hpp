#ifndef CgiProcessor_HPP_
#define CgiProcessor_HPP_

#include "IObserver.hpp"
#include "IRequestProcessor.hpp"
#include "LocationConfig.hpp"
#include "RequestVO.hpp"
#include "ResponseVO.hpp"

class CgiProcessor : public IRequestProcessor {
 public:
  CgiProcessor(const RequestVO &request, const LocationConfig *config,
               IObserver<ResponseVO> *ob);

 private:
  IObserver<ResponseVO> *ob_;
};

#endif
