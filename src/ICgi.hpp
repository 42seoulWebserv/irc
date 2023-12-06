#ifndef ICgi_HPP_
#define ICgi_HPP_

#define CGI_RECV_BUFFER 8192

#include "DataStream.hpp"
#include "Log.hpp"
#include "StringBuffer.hpp"

class ICgi {
 public:
  virtual ~ICgi(){};

  virtual int getFd() = 0;
  virtual StringBuffer &getRecvBuffer() = 0;
  virtual DataStream &getWriteBuffer() = 0;
  virtual void print(Log &logger, const std::string &msg) = 0;
};

#endif
