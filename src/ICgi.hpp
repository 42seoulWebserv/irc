#ifndef ICgi_HPP_
#define ICgi_HPP_

class ICgi {
 public:
  virtual ~ICgi(){};

  virtual void setFd(int &fd) = 0;
  virtual int getFd() = 0;
};

#endif
