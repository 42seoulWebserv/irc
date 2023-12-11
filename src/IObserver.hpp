#ifndef IObserver_HPP_
#define IObserver_HPP_

template <typename T>
class IObserver {
 public:
  virtual ~IObserver() {}
  virtual void onEvent(const T &p) = 0;
};

#endif
