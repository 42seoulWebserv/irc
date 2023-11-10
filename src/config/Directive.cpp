#include "Directive.hpp"

Directive::Directive() {}

Directive::Directive(const std::string &value) { this->key_ = value; }

Directive::Directive(const Directive &src) { *this = src; }

Directive &Directive::operator=(const Directive &rhs) {
  if (this == &rhs) return *this;
  this->key_ = rhs.key_;
  this->children_ = rhs.children_;
  this->values_ = rhs.values_;
  return *this;
}

Directive::~Directive() {}

std::string Directive::getKey() const { return key_; }

void Directive::setKey(const std::string &key) { key_ = key; }

std::vector<std::string> Directive::getValues() const { return values_; }

void Directive::setValues(const std::vector<std::string> &values) {
  values_ = values;
}

void Directive::addValues(const std::string &values) {
  values_.push_back(values);
}

const std::string Directive::getElementAtIndexValues(size_t index) const {
  if (index < values_.size()) {
    return values_[index];
  } else {
    throw std::invalid_argument("invalid index format");
  }
}

const std::vector<std::string>::iterator Directive::beginValues() {
  return values_.begin();
}

const std::vector<std::string>::iterator Directive::endValues() {
  return values_.end();
}

std::vector<Directive> Directive::getChildren() const { return children_; }

void Directive::setChildren(const std::vector<Directive> &children) {
  children_ = children;
}

void Directive::addChildren(const Directive &children) {
  children_.push_back(children);
}

const Directive Directive::getElementAtIndexChildren(size_t index) const {
  if (index < children_.size()) {
    return children_[index];
  } else {
    throw std::invalid_argument("invalid children format");
  }
}

const std::vector<Directive>::iterator Directive::beginChildren() {
  return children_.begin();
}

const std::vector<Directive>::iterator Directive::endChildren() {
  return children_.end();
}
