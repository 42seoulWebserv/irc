#ifndef SERVERCONNECTOR_HPP_
#define SERVERCONNECTOR_HPP_

#include "Connector.hpp"
#include "ClientConnector.hpp"

class ServerConnector : public Connector
{
public:
	ServerConnector(int kq);
	ServerConnector(const ServerConnector &src);
	ServerConnector &operator=(const ServerConnector &rhs);
	~ServerConnector();

	enum Connector::returnType handleEvent(const struct kevent &event);

	int kq_;
	int socket_;
private:
};

#endif
