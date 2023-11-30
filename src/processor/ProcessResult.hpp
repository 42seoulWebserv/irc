#ifndef ProcessResult_HPP_
#define ProcessResult_HPP_

#include "Request.hpp"
#include "Response.hpp"

class IProcessor;
class ProcessResult {
 public:
  ProcessResult();

  ProcessResult &setWriteOn(bool b);
  ProcessResult &setWriteOff(bool b);
  ProcessResult &setReadOn(bool b);
  ProcessResult &setReadOff(bool b);
  ProcessResult &setError(bool b);
  ProcessResult &setNextProcessor(IProcessor *nextProcessor);

  bool writeOn_;               // suggest write on
  bool writeOff_;              // suggest write off
  bool readOn_;                // suggest read on
  bool readOff_;               // suggest read off
  bool error_;                 // occur critical error (close socket)
  IProcessor *nextProcessor_;  // delete this and run next processor
};

#endif
