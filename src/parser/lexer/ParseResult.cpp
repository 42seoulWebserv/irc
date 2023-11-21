#include "ParseResult.hpp"

#include <stdexcept>

ParseResult::ParseResult()
    : name_(),
      values_(),
      children_(),
      depth_(0),
      isFlat_(false),
      isIgnore_(false) {}

ParseResult::ParseResult(const std::string &name)
    : name_(name),
      values_(),
      children_(),
      depth_(0),
      isFlat_(false),
      isIgnore_(false) {}

ParseResult::ParseResult(const std::string &name, int depth)
    : name_(name),
      values_(),
      children_(),
      depth_(depth),
      isFlat_(false),
      isIgnore_(false) {}

ParseResult::ParseResult(const std::string &name, int depth, bool isFlat,
                         bool isIgnore)
    : name_(name),
      values_(),
      children_(),
      depth_(depth),
      isFlat_(isFlat),
      isIgnore_(isIgnore) {}

ParseResult::ParseResult(const ParseResult &rhs)
    : name_(rhs.name_),
      values_(rhs.values_),
      children_(rhs.children_),
      depth_(rhs.depth_),
      isFlat_(rhs.isFlat_),
      isIgnore_(rhs.isIgnore_) {}

ParseResult &ParseResult::operator=(const ParseResult &rhs) {
  if (this == &rhs) return *this;
  name_ = rhs.name_;
  values_ = rhs.values_;
  children_ = rhs.children_;
  depth_ = rhs.depth_;
  isFlat_ = rhs.isFlat_;
  isIgnore_ = rhs.isIgnore_;
  return *this;
}

void ParseResult::setName(const std::string &name) { name_ = name; }

const std::string &ParseResult::getName() const { return name_; }

void ParseResult::addValue(const std::string &value) {
  values_.push_back(value);
}

const std::vector<std::string> &ParseResult::getValues() const {
  return values_;
}

const std::string &ParseResult::getValue(int i) const {
  return getValues().at(i);
}

int ParseResult::getDepth() const { return depth_; }

void ParseResult::push(const ParseResult &child) { children_.push_back(child); }

ParseResult ParseResult::pop() {
  if (children_.size() == 0) throw std::out_of_range("out of range");
  ParseResult result = children_[children_.size() - 1];
  children_.pop_back();
  return result;
}

const std::vector<ParseResult> &ParseResult::getChildren() const {
  return children_;
}

const ParseResult &ParseResult::getChild(int i) const {
  return getChildren().at(i);
}

const std::string ParseResult::toString() const {
  if (getChildren().size() == 0) {
    std::string res = name_ + ":";
    const std::vector<std::string> &values = getValues();
    for (size_t i = 0; i < values.size(); i++) {
      res += " " + values[i];
    }
    return std::string(depth_ * 2, ' ') + res;
  }
  std::string res = std::string(depth_ * 2, ' ');
  if (name_.empty())
    res += "[\n";
  else
    res += name_ + " [\n";
  for (size_t i = 0; i < getChildren().size(); i++) {
    res += getChildren().at(i).toString() + "\n";
  }
  res += std::string(depth_ * 2, ' ') + "]";
  return res;
}

const ParseResult ParseResult::trim() const {
  ParseResult result(name_, depth_, isFlat_, isIgnore_);
  if (result.isFlat_) {
    ParseResult::insertFlatValues(result.values_, *this);
    return result;
  }
  for (std::vector<std::string>::const_iterator it = values_.begin();
       it != values_.end(); it++) {
    result.values_.push_back(*it);
  }
  for (size_t i = 0; i < children_.size(); i++) {
    if (children_[i].isIgnore_ == false) result.push(children_[i].trim());
  }
  return result;
}

void ParseResult::insertFlatValues(std::vector<std::string> &values,
                                   const ParseResult &target) {
  for (std::vector<std::string>::const_iterator it = target.values_.begin();
       it != target.values_.end(); it++) {
    values.push_back(*it);
  }
  for (size_t i = 0; i < target.children_.size(); i++) {
    if (target.children_[i].isIgnore_ == false)
      ParseResult::insertFlatValues(values, target.children_[i]);
  }
}
