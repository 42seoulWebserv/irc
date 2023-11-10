#ifndef DIRECTIVE_HPP
#define DIRECTIVE_HPP

#include <iostream>
#include <map>
#include <vector>

class Directive {
 public:
  Directive();
  Directive(const std::string &value);
  Directive(const Directive &src);
  Directive &operator=(const Directive &rhs);
  ~Directive();

  std::string getKey() const;
  void setKey(const std::string &key);
  std::vector<std::string> getValues() const;
  void setValues(const std::vector<std::string> &values);
  void addValues(const std::string &values);
  const std::string getElementAtIndexValues(size_t index) const;
  const std::vector<std::string>::iterator beginValues();
  const std::vector<std::string>::iterator endValues();
  std::vector<Directive> getChildren() const;
  void setChildren(const std::vector<Directive> &children);
  void addChildren(const Directive &children);
  const Directive getElementAtIndexChildren(size_t index) const;
  const std::vector<Directive>::iterator beginChildren();
  const std::vector<Directive>::iterator endChildren();

 private:
  std::string key_;
  std::vector<std::string> values_;
  std::vector<Directive> children_;
};

#endif
