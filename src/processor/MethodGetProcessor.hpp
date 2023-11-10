#ifndef MethodGetProcessor_HPP_
#define MethodGetProcessor_HPP_

#include "FileReadEventController.hpp"
#include "IObserver.hpp"
#include "IRequestProcessor.hpp"
#include "LocationConfig.hpp"
#include "RequestVO.hpp"
#include "ResponseVO.hpp"

class MethodGetProcessor : public IRequestProcessor,
                           IObserver<FileReadEventController::Event> {
 public:
  MethodGetProcessor(const RequestVO &request, const LocationConfig *config,
                     int kq, IObserver<ResponseVO> *ob);
  void onEvent(const FileReadEventController::Event &event);

 private:
  IObserver<ResponseVO> *ob_;
};

#endif
