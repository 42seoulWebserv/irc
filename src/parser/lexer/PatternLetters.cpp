#include "PatternLetters.hpp"

PatternLetters::PatternLetters(std::string name, std::string letters)
    : Pattern(name), letters_(letters), ignore_(false) {}

PatternLetters::PatternLetters(const PatternLetters &rhs)
    : Pattern(rhs), letters_(rhs.letters_), ignore_(rhs.ignore_) {}

PatternLetters &PatternLetters::operator=(const PatternLetters &rhs) {
  if (this == &rhs) return *this;
  setName(rhs.getName());
  ignore_ = rhs.ignore_;
  letters_ = rhs.letters_;
  return *this;
}

PatternLetters::~PatternLetters() {}

PatternLetters PatternLetters::setIgnore(bool ignore) const {
  PatternLetters result(*this);
  result.ignore_ = ignore;
  return result;
}

Pattern *PatternLetters::clone() const { return new PatternLetters(*this); }

bool PatternLetters::compile(std::stringstream &ss, ParseResult &res,
                             bool debug) const {
  std::streampos begin = ss.tellg();
  if (ss.fail()) return fail(debug, "PatternLetters fail to begin");

  ss >> std::ws;

  ParseResult result(getAlias(), res.getDepth() + 1, isFlatResult(),
                     isIgnoreResult());
  std::string str;
  char c;
  while (ss.eof() == false) {
    ss >> c;
    if (ss.fail()) break;

    if (std::isspace(c)) {
      ss.unget();
      break;
    }

    size_t pos = letters_.find(c);
    if (ignore_ == true && pos != std::string::npos) {
      ss.unget();
      break;
    }
    if (ignore_ == false && pos == std::string::npos) {
      ss.unget();
      break;
    }
    str.push_back(c);
  }
  if (str.empty()) {
    ss.seekg(begin);
    return false;
  }
  if (debug) this->debug(result.getDepth(), "[ " + str + " ]");
  result.addValue(str);
  res.push(result);
  return true;
}
