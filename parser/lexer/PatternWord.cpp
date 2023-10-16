#include "PatternWord.hpp"

PatternWord::PatternWord(std::string name, std::string word)
    : Pattern(name), word_(word)
{
}

PatternWord::PatternWord(const PatternWord &rhs)
    : Pattern(rhs), word_(rhs.word_)
{
}

PatternWord &PatternWord::operator=(const PatternWord &rhs)
{
    if (this == &rhs) return *this;
    setName(rhs.getName());
    word_ = rhs.word_;
    return *this;
}

PatternWord::~PatternWord()
{
}

Pattern *PatternWord::clone() const
{
    return new PatternWord(*this);
}

bool PatternWord::compile(std::stringstream &ss, ParseResult &res, bool debug) const
{
    std::streampos begin = ss.tellg();
    if (ss.fail()) return fail(debug, "PatternWord fail to begin");

    if (word_.empty()) return false;
    ss >> std::ws;

    ParseResult result(getAlias(), res.getDepth() + 1, isFlatResult(), isIgnoreResult());
    std::string str;
    char c;
    for (size_t i = 0; i < word_.size(); i++) {
        ss >> c;
        if (ss.fail() || c != word_.at(i)) {
            ss.seekg(begin);
            return false;
        }
    }
    result.addValue(word_);
    res.push(result);
    return true;
}
