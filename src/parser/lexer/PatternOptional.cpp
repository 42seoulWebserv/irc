#include "Parser.hpp"
#include "PatternOptional.hpp"

PatternOptional::PatternOptional(std::string name) :
    Pattern(name), min_(0), max_(99999)
{
}

PatternOptional::PatternOptional(const PatternOptional &rhs) :
    Pattern(rhs), patterns_(rhs.patterns_), min_(rhs.min_), max_(rhs.max_)
{
}

PatternOptional &PatternOptional::operator=(PatternOptional &rhs)
{
    if (this == &rhs) return *this;

    setName(rhs.getName());
    setParser(rhs.parser_);
    min_ = rhs.min_;
    max_ = rhs.max_;
    for (size_t i = 0; i < rhs.patterns_.size(); i++) {
        patterns_.push_back(rhs.patterns_[i]);
    }
    return *this;
}

PatternOptional::~PatternOptional()
{
}

PatternOptional PatternOptional::add(const std::string &pattern) const
{
    PatternOptional result(*this);
    result.patterns_.push_back(pattern);
    return result;
}

PatternOptional PatternOptional::setMin(unsigned int min) const
{
    PatternOptional result(*this);
    result.min_ = min;
    return result;
}

PatternOptional PatternOptional::setMax(unsigned int max) const
{
    PatternOptional result(*this);
    result.max_ = max;
    return result;
}

Pattern *PatternOptional::clone() const
{
    return new PatternOptional(*this);
}

bool PatternOptional::compile(std::stringstream &ss, ParseResult &res, bool debug) const
{
    std::streampos begin = ss.tellg();
    if (ss.fail()) return fail(debug, "PatternOptional fail to begin");

    std::streampos cursor = begin;
    ParseResult result(getAlias(), res.getDepth() + 1, isFlatResult(), isIgnoreResult());
    if (debug) this->debug(result.getDepth(), "------ op ------");
    for (size_t i = 0; i < patterns_.size(); i++) {
        const Pattern &pattern = getPattern(patterns_[i]);
        if (debug) this->debug(result.getDepth(), pattern.getName() + " try");
        if (pattern.compile(ss, result, debug)) {
            if (debug) this->debug(result.getDepth(), pattern.getName() + " matched");
            i = -1;
            cursor = ss.tellg();
            if (ss.fail() || result.getChildren().size() >= max_) break;
        }
        else {
            ss.clear();
            ss.seekg(cursor);
            if (debug) this->debug(result.getDepth(), pattern.getName() + " not matched");
        }
    }
    if (result.getChildren().size() < min_) {
        if (debug) this->debug(result.getDepth(), "--- not match ---");
        ss.seekg(begin);
        return false;
    }
    if (result.getChildren().size() > max_) {
        if (debug) this->debug(result.getDepth(), "--- not match ---");
        ss.seekg(begin);
        return false;
    }
    if (debug) this->debug(result.getDepth(), "--- complete ---");
    res.push(result);
    return true;
}
