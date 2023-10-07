#ifndef BlockDirective_HPP
#define BlockDirective_HPP

#include "Directive.hpp"

class BlockDirective : public Directive
{
public:
	std::vector<Directive> children;
};

#endif