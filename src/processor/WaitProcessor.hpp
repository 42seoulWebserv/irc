#ifndef WaitProcessor_HPP_
#define WaitProcessor_HPP_

#include "IProcessor.hpp"

class WaitProcessor : public IProcessor {
 public:
  ProcessResult process();
};

#endif