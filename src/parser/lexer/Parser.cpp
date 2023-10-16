#include <vector>
#include "Parser.hpp"

Parser::Parser()
{
}

Parser::Parser(const Parser &rhs)
{
    typedef std::map<const std::string, const Pattern*>::const_iterator iter;
    for (iter it = rhs.map_.begin(); it != rhs.map_.end(); it++) {
        Pattern *pattern = it->second->clone();
        pattern->setParser(this);
        map_[it->first] = pattern;
    }
}

Parser &Parser::operator=(const Parser &rhs)
{
    if (this == &rhs) return *this;

    typedef std::map<const std::string, const Pattern*>::const_iterator iter;
    for (iter it = rhs.map_.begin(); it != rhs.map_.end(); it++) {
        Pattern *pattern = it->second->clone();
        pattern->setParser(this);
        map_[it->first] = pattern;
    }
    return *this;
}

Parser::~Parser()
{
    typedef std::map<const std::string, const Pattern*>::const_iterator iter;
    for (iter it = map_.begin(); it != map_.end(); it++) {
        delete map_[it->first];
    }
}

ParseResult Parser::compile(const std::string &root, const std::string &str)
{
    std::stringstream ss(str);
    ss >> std::noskipws;
    ParseResult result(root, -1);
    if ((*this)[root].compile(ss, result, false) == false)
        throw std::invalid_argument("invalid");
    return result.getChildren().at(0).trim();
}

Parser &Parser::add(const Pattern &pattern)
{
    if (map_.find(pattern.getName()) != map_.end()) {
        delete map_.at(pattern.getName());
    }
    Pattern *clone = pattern.clone();
    clone->setParser(this);
    map_[pattern.getName()] = clone;
    return *this;
}

const Pattern &Parser::operator[](const std::string &name)
{
    try {
        return *(map_.at(name));
    } catch(std::out_of_range &e) {
        throw std::out_of_range(name);
    }
}
