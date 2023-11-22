#ifndef WaitProcessor_HPP_
#define WaitProcessor_HPP_

#include "IRequestProcessor.hpp"

class WaitProcessor : public IRequestProcessor {
 public:
  ProcessResult process();
};

#endif