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
    std::vector<Directive> children;

};

#endif