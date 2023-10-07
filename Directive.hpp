#ifndef A_HPP
#define A_HPP

#include <iostream>
#include <map>
#include <vector>

class Directive
{
public:
	std::string key;
	std::vector<std::string> values;
};

class SimpleDirective : public Directive
{
public:
	SimpleDirective(std::string key, std::string value);
};

class BlockDirective : public Directive
{
public:
	std::vector<Directive> children;
};

#endif