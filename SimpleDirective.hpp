#ifndef SimpleDirective_HPP
#define SimpleDirective_HPP

#include "Directive.hpp"

class SimpleDirective : public Directive
{
public:
	SimpleDirective(std::string key);
	void addValue(std::string value);
};

#endif