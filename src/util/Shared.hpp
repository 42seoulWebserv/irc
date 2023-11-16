#ifndef Shared_HPP_
#define Shared_HPP_

#include <iostream>

template <typename T>
class Shared {
 public:
  explicit Shared(T *value) : value(value), counter(new int(1)){};
  Shared(const Shared<T> &rhs) : value(rhs.value), counter(rhs.counter) {
    (*counter)++;
  };
  Shared &operator=(const Shared<T> &rhs) {
    if (this == &rhs) return *this;
    decreaseCounter();
    value = rhs.value;
    counter = rhs.counter;
    (*counter)++;
    return *this;
  };
  ~Shared() { decreaseCounter(); };
  T *ptr() { return value; };

 private:
  void decreaseCounter() {
    (*counter)--;
    if (*counter == 0) {
      delete value;
      delete counter;
    }
  };
  T *value;
  int *counter;
};

#endif
