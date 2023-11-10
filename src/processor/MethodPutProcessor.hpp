#ifndef MethodPutProcessor_HPP_
#define MethodPutProcessor_HPP_

#include "FileReadEventController.hpp"
#include "FileWriteEventController.hpp"
#include "IObserver.hpp"
#include "IRequestProcessor.hpp"
#include "LocationConfig.hpp"
#include "RequestVO.hpp"
#include "ResponseVO.hpp"

class MethodPutProcessor : public IRequestProcessor,
                           IObserver<FileReadEventController::Event>,
                           IObserver<FileWriteEventController::Event> {
public:
  MethodPutProcessor(const RequestVO &request, const LocationConfig *config,
                     int kq, IObserver<ResponseVO> *ob);

  void onEvent(const FileReadEventController::Event &event, int kq);
  void onEvent(const FileWriteEventController::Event &event);

private:
  IObserver<ResponseVO> *ob_;
};

#endif
