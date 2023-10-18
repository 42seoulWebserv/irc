#ifndef CONNECTOR_HPP_
#define CONNECTOR_HPP_

class Connector
{
public:
	Connector();
	Connector(const Connector &src);
	Connector &operator=(const Connector &rhs);
	virtual ~Connector();

	enum returnType {
		OK,
		FAIL
	};

	virtual enum returnType handleEvent(const struct kevent &event) = 0;

private:
};

#endif