#ifndef PatternLetters_HPP
#define PatternLetters_HPP

#include "Pattern.hpp"

class PatternLetters : public Pattern
{
public:
    PatternLetters(std::string name, std::string letters);
    PatternLetters(const PatternLetters &rhs);
    PatternLetters &operator=(const PatternLetters &rhs);
    ~PatternLetters();

    PatternLetters setIgnore(bool ignore) const;

    Pattern *clone() const;
    bool compile(std::stringstream &ss, ParseResult &res, bool debug) const;

private:
    std::string letters_;
    bool ignore_;

};

#endif
