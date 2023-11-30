#ifndef IClient_HPP_
#define IClient_HPP_

#include <vector>

#include "DataStream.hpp"
#include "LocationConfig.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "StringBuffer.hpp"

class IClient {
 public:
  virtual ~IClient(){};
  virtual const Request &getRequest() const = 0;
  virtual void setRequest(const Request &request) = 0;
  virtual const Response &getResponse() const = 0;
  virtual void setResponse(const Response &response) = 0;
  virtual void setResponseStatusCode(int code) = 0;
  virtual void setResponseHeader(const std::string &key,
                                 const std::string &value) = 0;
  virtual void setBody(const std::string &body) = 0;
  virtual DataStream &getDataStream() = 0;
  virtual StringBuffer &getRecvBuffer() = 0;
  virtual const LocationConfig *getLocationConfig() = 0;
};

#endif
