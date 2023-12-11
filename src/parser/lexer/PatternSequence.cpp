#include "PatternSequence.hpp"

#include "Parser.hpp"

PatternSequence::PatternSequence(std::string name) : Pattern(name) {}

PatternSequence::PatternSequence(const PatternSequence &rhs)
    : Pattern(rhs), patterns_(rhs.patterns_) {}

PatternSequence &PatternSequence::operator=(const PatternSequence &rhs) {
  if (this == &rhs) return *this;

  setName(rhs.getName());
  setParser(rhs.parser_);
  for (size_t i = 0; i < rhs.patterns_.size(); i++) {
    patterns_.push_back(rhs.patterns_[i]);
  }
  return *this;
}

PatternSequence::~PatternSequence() {}

PatternSequence PatternSequence::add(const std::string &pattern) const {
  PatternSequence result(*this);
  result.patterns_.push_back(pattern);
  return result;
}

Pattern *PatternSequence::clone() const { return new PatternSequence(*this); }

bool PatternSequence::compile(std::stringstream &ss, ParseResult &res,
                              bool debug) const {
  std::streampos begin = ss.tellg();
  if (ss.fail()) return fail(debug, "PatternSequence fail to begin");

  ParseResult result(getAlias(), res.getDepth() + 1, isFlatResult(),
                     isIgnoreResult());
  if (debug) this->debug(result.getDepth(), "------ se ------");
  for (size_t i = 0; i < patterns_.size(); i++) {
    const Pattern &pattern = getPattern(patterns_[i]);
    if (debug) this->debug(result.getDepth(), pattern.getName() + " try");
    if (pattern.compile(ss, result, debug) == false) {
      if (debug)
        this->debug(result.getDepth(), pattern.getName() + " not matched");
      ss.clear();
      ss.seekg(begin);
      return false;
    } else if (debug)
      this->debug(result.getDepth(), pattern.getName() + " matched");
  }
  if (debug) this->debug(result.getDepth(), "--- complete ---");
  res.push(result);
  return true;
}
