#ifndef CLIENTEVENTCONTROLLER_HPP_
#define CLIENTEVENTCONTROLLER_HPP_

#include <map>
#include <string>
#include <ostream>

#include "EventController.hpp"

#define BUFF_SIZE 4
/* server(port) 또는 client가 보낸 요청을 수행하는 클래스 */
class ClientEventController : public EventController
{
public:
	enum READ_STATUS {
		START_LINE,
		HEADER,
		BODY
	};

	ClientEventController(int kq, int clientSocket);
	ClientEventController(const ClientEventController &src);
	ClientEventController &operator=(const ClientEventController &rhs);
	virtual ~ClientEventController();

	enum EventController::returnType handleEvent(const struct kevent &event);
	void parseHeaderLineByLine(std::string str);
	void parseStartLine(std::string str);
	void printParseResult();

	bool isValidReq_;
	enum READ_STATUS status_;
	int kq_;
	int clientSocket_;
	std::string method_;
	std::string uri_;
	std::string version_;
	std::map<std::string, std::string> headers_;
	std::string body_;
	std::string headerBuffer_;
	std::string bodyBuffer_;

private:
	enum EventController::returnType clientRead(const struct kevent &event);
	enum EventController::returnType clientWrite(const struct kevent &event);
	enum EventController::returnType clientTimeout(const struct kevent &event);
};
/*
// 우선 key에 공백 있으면 안됨.
 TEST: 123 // 맨앞 공백
TEST :123 // key에 :가 붙어있지않음
:TEST: 123 // 맨앞에 : 
TE ST: 123 // key 사이에 공백
*/

std::ostream& operator<<(std::ostream& o, const std::map<std::string, std::string>& rhs);

#endif
