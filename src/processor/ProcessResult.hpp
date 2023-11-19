#ifndef ProcessResult_HPP_
#define ProcessResult_HPP_

#include "Request.hpp"
#include "Response.hpp"

class IRequestProcessor;
class ProcessResult {
 public:
  ProcessResult();

  ProcessResult &setWriteOn(bool b);
  ProcessResult &setWriteOff(bool b);
  ProcessResult &setReadOn(bool b);
  ProcessResult &setReadOff(bool b);
  ProcessResult &setError(bool b);
  ProcessResult &setSpendReadBuffer(unsigned spendReadBuffer);
  ProcessResult &setStatus(int status);
  ProcessResult &setNextProcessor(IRequestProcessor *nextProcessor);
  ProcessResult &setRequest(const Request *request);
  ProcessResult &setResponse(const Response *response);

  unsigned writeOn_ : 1;              // suggest write on
  unsigned writeOff_ : 1;             // suggest write off
  unsigned readOn_ : 1;               // suggest read on
  unsigned readOff_ : 1;              // suggest read off
  unsigned error_ : 1;                // occur critical error (close socket)
  unsigned spendReadBuffer_;          // spend read size from client recvBuffer
  int status_;                        // status code
  IRequestProcessor *nextProcessor_;  // delete this and run next processor
  const Request *request;             // cover this
  const Response *response;           // cover this
};

#endif
