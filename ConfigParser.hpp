#ifndef Parser_HPP
#define Parser_HPP

#include "BlockDirective.hpp"

class Parser
{
public:
	Parser(BlockDirective directive);
	bool fail();
	BlockDirective getRootConfig();
};

#endif