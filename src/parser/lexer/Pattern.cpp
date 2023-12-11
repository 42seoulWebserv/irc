#include "Pattern.hpp"

#include "Parser.hpp"

Pattern::Pattern(std::string name)
    : parser_(NULL),
      name_(name),
      alias_(name),
      isFlatResult_(false),
      isIgnoreResult_(false) {}

Pattern::Pattern(const Pattern &rhs)
    : parser_(rhs.parser_),
      name_(rhs.name_),
      alias_(rhs.alias_),
      isFlatResult_(rhs.isFlatResult_),
      isIgnoreResult_(rhs.isIgnoreResult_) {}

Pattern &Pattern::operator=(const Pattern &rhs) {
  if (this == &rhs) return *this;
  parser_ = rhs.parser_;
  name_ = rhs.name_;
  alias_ = rhs.alias_;
  isFlatResult_ = rhs.isFlatResult_;
  isIgnoreResult_ = rhs.isIgnoreResult_;
  return *this;
}

Pattern::~Pattern() {}

const std::string &Pattern::getName() const { return name_; }

const std::string &Pattern::getAlias() const { return alias_; }

Pattern &Pattern::setAlias(const std::string &alias) {
  alias_ = alias;
  return *this;
}

bool Pattern::fail(bool debug, const std::string &message) const {
  if (debug) std::cout << message << std::endl;
  return false;
}

void Pattern::debug(int depth, const std::string &message) const {
  std::cout << std::string(depth * 2, ' ') << getName() << ": " << message
            << std::endl;
}

void Pattern::setParser(Parser *parser) { parser_ = parser; }

Pattern &Pattern::flatResult() {
  isFlatResult_ = true;
  return *this;
}

bool Pattern::isFlatResult() const { return isFlatResult_; }

Pattern &Pattern::ignoreResult() {
  isIgnoreResult_ = true;
  return *this;
}

bool Pattern::isIgnoreResult() const { return isIgnoreResult_; }

Pattern &Pattern::setName(const std::string &name) {
  name_ = name;
  return *this;
}

const Pattern &Pattern::getPattern(const std::string &name) const {
  return (*parser_)[name];
}
