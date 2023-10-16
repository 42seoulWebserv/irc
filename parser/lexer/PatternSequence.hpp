#ifndef PatternSequence_HPP
#define PatternSequence_HPP

#include <vector>
#include "Pattern.hpp"

class PatternSequence : public Pattern
{
public:
    PatternSequence(std::string name);
    PatternSequence(const PatternSequence &rhs);
    PatternSequence &operator=(const PatternSequence &rhs);
    ~PatternSequence();

    PatternSequence add(const std::string &pattern) const;

    Pattern *clone() const;
    bool compile(std::stringstream &ss, ParseResult &res, bool debug) const;

private:
    std::vector<const std::string> patterns_;

};

#endif
