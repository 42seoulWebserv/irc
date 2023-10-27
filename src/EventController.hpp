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
		SUCCESS, // 정상적인 페이지 반환
		FAIL, // response X, connect 끊기
		ERROR, // error page 준비
		PENDING, // 계속 사용
	};

	virtual enum returnType handleEvent(const struct kevent &event) = 0;

private:
};

#endif