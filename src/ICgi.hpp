#ifndef ICgi_HPP_
#define ICgi_HPP_

#define CGI_RECV_BUFFER 8192

#include "DataStream.hpp"
#include "StringBuffer.hpp"

class ICgi {
 public:
  virtual ~ICgi(){};

  virtual void setFd(int &fd) = 0;
  virtual int getFd() = 0;
  virtual StringBuffer &getRecvBuffer() = 0;
  virtual void end() = 0;
  virtual DataStream &getWriteBuffer() = 0;
};

#endif
