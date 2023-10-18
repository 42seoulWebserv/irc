#ifndef CLIENTCONNECTOR_HPP_
#define CLIENTCONNECTOR_HPP_

#include "Connector.hpp"

#define BUFF_SIZE 1024
/* server(port) 또는 client가 보낸 요청을 수행하는 클래스 */
/* client-connector 이름 바꾸기 */
class ClientConnector : public Connector
{
public:
	ClientConnector(int kq, int clientSocket);
	ClientConnector(const ClientConnector &src);
	ClientConnector &operator=(const ClientConnector &rhs);
	virtual ~ClientConnector();

	enum Connector::returnType handleEvent(const struct kevent &event);

	int kq_;
	int clientSocket_;
private:
};

#endif
