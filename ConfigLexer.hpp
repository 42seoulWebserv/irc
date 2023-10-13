#ifndef Lexer_HPP
#define Lexer_HPP

#include <iostream>
#include <sstream>
#include "parser/lexer/ParseResult.hpp"
#include "Directive.hpp"

class ConfigLexer
{
public:
    static const Directive run(const std::string raw);

private:
    static const Directive parseResultToDirective(const ParseResult &res);
};

#endif
