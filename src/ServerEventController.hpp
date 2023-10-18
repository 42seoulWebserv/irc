#ifndef SERVEREVENTCONTROLLER_HPP_
#define SERVEREVENTCONTROLLER_HPP_

#include "EventController.hpp"
#include "ClientEventController.hpp"

class ServerEventController : public EventController
{
public:
	ServerEventController(int kq);
	ServerEventController(const ServerEventController &src);
	ServerEventController &operator=(const ServerEventController &rhs);
	~ServerEventController();

	enum EventController::returnType handleEvent(const struct kevent &event);

	int kq_;
	int socket_;
private:
};

#endif
