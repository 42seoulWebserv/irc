#ifndef CLIENTEVENTCONTROLLER_HPP_
#define CLIENTEVENTCONTROLLER_HPP_

#include "EventController.hpp"

#define BUFF_SIZE 1024
/* server(port) 또는 client가 보낸 요청을 수행하는 클래스 */
class ClientEventController : public EventController
{
public:
	ClientEventController(int kq, int clientSocket);
	ClientEventController(const ClientEventController &src);
	ClientEventController &operator=(const ClientEventController &rhs);
	virtual ~ClientEventController();

	enum EventController::returnType handleEvent(const struct kevent &event);

	int kq_;
	int clientSocket_;
private:
	enum EventController::returnType clientRead(const struct kevent &event);
	enum EventController::returnType clientWrite(const struct kevent &event);
	enum EventController::returnType clientTimeout(const struct kevent &event);

};

#endif
