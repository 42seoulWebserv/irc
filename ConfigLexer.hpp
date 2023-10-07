#ifndef Lexer_HPP
#define Lexer_HPP

#include <string>

class Lexer
{
public:
	Lexer(std::string configStr);
	BlockDirective getRootRedirective();
	bool fail();
	.
	.
private:
	BlockDirective lexedResult;
};

#endif