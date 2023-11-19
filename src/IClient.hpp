#ifndef IClient_HPP_
#define IClient_HPP_

#include <vector>

#include "DataStream.hpp"
#include "LocationConfig.hpp"
#include "Request.hpp"
#include "Response.hpp"

class IClient {
 public:
  virtual ~IClient(){};
  virtual const Request &getRequest() const = 0;
  virtual const Response &getResponse() const = 0;
  virtual DataStream &getDataStream() = 0;
  virtual const std::vector<char> &getRecvBuffer() const = 0;
  virtual const LocationConfig *getLocationConfig() = 0;
};

#endif
