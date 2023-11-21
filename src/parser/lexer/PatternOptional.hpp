#ifndef PatternOptional_HPP
#define PatternOptional_HPP

#include <vector>

#include "Pattern.hpp"
class PatternOptional : public Pattern {
 public:
  PatternOptional(std::string name);
  PatternOptional(const PatternOptional &rhs);
  PatternOptional &operator=(PatternOptional &rhs);
  ~PatternOptional();

  PatternOptional add(const std::string &pattern) const;
  PatternOptional setMin(unsigned int min) const;
  PatternOptional setMax(unsigned int max) const;

  Pattern *clone() const;
  bool compile(std::stringstream &ss, ParseResult &res, bool debug) const;

 private:
  std::vector<std::string> patterns_;
  unsigned int min_;
  unsigned int max_;
};

#endif
