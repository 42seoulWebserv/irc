#ifndef ICancelible_HPP_
#define ICancelible_HPP_

class ICancelible {
 public:
  virtual ~ICancelible(){};
  virtual void cancel() = 0;
};

#endif