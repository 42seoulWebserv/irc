#ifndef EVENTCONTROLLER_HPP_
#define EVENTCONTROLLER_HPP_

/* event hadling
** kqueue가 발생시키는 event를 handle
*/
class EventController
{
public:
	EventController();
	EventController(const EventController &src);
	EventController &operator=(const EventController &rhs);
	virtual ~EventController();

	enum returnType {
		SUCCESS,
		FAIL,
		PENDING,
		TIMEOUT,
	};

	virtual enum returnType handleEvent(const struct kevent &event) = 0;

private:
};

#endif