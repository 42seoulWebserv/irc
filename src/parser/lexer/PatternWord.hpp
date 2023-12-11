#ifndef PatternWord_HPP
#define PatternWord_HPP

#include "Pattern.hpp"

class PatternWord : public Pattern {
 public:
  PatternWord(std::string name, std::string word);
  PatternWord(const PatternWord &rhs);
  PatternWord &operator=(const PatternWord &rhs);
  ~PatternWord();

  Pattern *clone() const;
  bool compile(std::stringstream &ss, ParseResult &res, bool debug) const;

 private:
  std::string word_;
};

#endif
