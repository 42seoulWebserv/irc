#ifndef ICancelable_HPP_
#define ICancelable_HPP_

class ICancelable {
 public:
  virtual ~ICancelable(){};
  virtual void cancel() = 0;
};

#endif