#ifndef MethodPostProcessor_HPP_
#define MethodPostProcessor_HPP_

#include "FileWriteEventController.hpp"
#include "IObserver.hpp"
#include "IRequestProcessor.hpp"
#include "LocationConfig.hpp"
#include "RequestVO.hpp"
#include "ResponseVO.hpp"

class MethodPostProcessor : public IRequestProcessor,
                            IObserver<FileWriteEventController::Event> {
public:
  MethodPostProcessor(const RequestVO &request, const LocationConfig *config,
                      int kq, IObserver<ResponseVO> *ob);
  void onEvent(const FileWriteEventController::Event &event);

private:
  IObserver<ResponseVO> *ob_;
};

#endif
