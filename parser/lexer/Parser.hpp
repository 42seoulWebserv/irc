#ifndef Parser_HPP
#define Parser_HPP

#include <sstream>
#include <iostream>
#include <map>
#include "Pattern.hpp"
#include "ParseResult.hpp"

class Parser
{
public:
    Parser();
    Parser(const Parser &rhs);
    Parser &operator=(const Parser &rhs);
    ~Parser();
    ParseResult compile(const std::string &root, const std::string &str);
    Parser &add(const Pattern &pattern);
    const Pattern &operator[](const std::string &name);

private:
    std::map<const std::string, const Pattern*> map_;

};

#endif
