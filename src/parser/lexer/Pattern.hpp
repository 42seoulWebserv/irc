#ifndef Pattern_HPP
#define Pattern_HPP

#include <iostream>
#include <sstream>

#include "ParseResult.hpp"

class Parser;
class Pattern {
 public:
  Pattern(std::string name);
  Pattern(const Pattern &rhs);
  Pattern &operator=(const Pattern &rhs);
  virtual ~Pattern();

  const std::string &getName() const;
  const std::string &getAlias() const;
  Pattern &setAlias(const std::string &alias);
  bool fail(bool debug, const std::string &message) const;
  void debug(int depth, const std::string &message) const;
  void setParser(Parser *parser);
  Pattern &flatResult();
  bool isFlatResult() const;
  Pattern &ignoreResult();
  bool isIgnoreResult() const;

  virtual Pattern *clone() const = 0;
  virtual bool compile(std::stringstream &ss, ParseResult &res,
                       bool debug) const = 0;

 protected:
  Pattern &setName(const std::string &name);
  const Pattern &getPattern(const std::string &name) const;
  Parser *parser_;

 private:
  std::string name_;
  std::string alias_;
  bool isFlatResult_;
  bool isIgnoreResult_;
};

#endif
