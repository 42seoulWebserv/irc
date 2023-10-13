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

Directive::~Directive() {

}

