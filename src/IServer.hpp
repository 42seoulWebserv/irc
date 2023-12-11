#ifndef IServer_HPP_
#define IServer_HPP_

class IServer {
 public:
  virtual ~IServer(){};
  virtual int acceptClient() = 0;
  virtual const std::vector<ServerConfig *> &getServerConfigs() const = 0;
};

#endif
