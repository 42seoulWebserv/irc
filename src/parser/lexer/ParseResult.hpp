#ifndef ParseResult_HPP
#define ParseResult_HPP

#include <string>
#include <vector>

class ParseResult {
 public:
  ParseResult();
  ParseResult(const std::string &name);
  ParseResult(const std::string &name, int depth);
  ParseResult(const std::string &name, int depth, bool isFlat, bool isIgnore);
  ParseResult(const ParseResult &rhs);
  ParseResult &operator=(const ParseResult &rhs);
  void setName(const std::string &name);
  const std::string &getName() const;
  void addValue(const std::string &value);
  const std::vector<std::string> &getValues() const;
  const std::string &getValue(int i) const;
  int getDepth() const;
  void push(const ParseResult &child);
  ParseResult pop();
  const std::vector<ParseResult> &getChildren() const;
  const ParseResult &getChild(int i) const;
  const std::string toString() const;
  const ParseResult trim() const;

 private:
  static void insertFlatValues(std::vector<std::string> &values,
                               const ParseResult &target);

  std::string name_;
  std::vector<std::string> values_;
  std::vector<ParseResult> children_;
  int depth_;
  bool isFlat_;
  bool isIgnore_;
};

#endif
