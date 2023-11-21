#include "ConfigLexer.hpp"

#include <exception>
#include <vector>

#include "ParseResult.hpp"
#include "Parser.hpp"
#include "PatternLetters.hpp"
#include "PatternOptional.hpp"
#include "PatternSequence.hpp"
#include "PatternWord.hpp"

const Directive ConfigLexer::run(const std::string raw) {
  if (raw.empty()) {
    return Directive();
  }

  Parser configParser =
      Parser()

          // words
          .add(PatternLetters("word", "{};").setIgnore(true))
          .add(PatternOptional("words").add("word"))
          .add(PatternOptional("words1~")
                   .add("word")
                   .setMin(1)
                   .setAlias("words")
                   .flatResult())
          .add(PatternOptional("words1")
                   .add("word")
                   .setMin(1)
                   .setMax(1)
                   .setAlias("words")
                   .flatResult())
          .add(PatternOptional("words2~")
                   .add("word")
                   .setMin(2)
                   .setAlias("words")
                   .flatResult())
          .add(PatternOptional("words2")
                   .add("word")
                   .setMin(2)
                   .setMax(2)
                   .setAlias("words")
                   .flatResult())

          // reserved letters
          .add(PatternWord("simpleEnd", ";").ignoreResult())
          .add(PatternWord("blockStart", "{").ignoreResult())
          .add(PatternWord("blockEnd", "}").ignoreResult())

          // directive words
          .add(PatternWord("http", "http").setAlias("key"))
          .add(PatternWord("server", "server").setAlias("key"))
          .add(PatternWord("location", "location").setAlias("key"))
          .add(PatternWord("clientMaxContentSize", "client_max_body_size")
                   .setAlias("key"))
          .add(PatternWord("root", "root").setAlias("key"))
          .add(PatternWord("serverName", "server_name").setAlias("key"))
          .add(PatternWord("listen", "listen").setAlias("key"))
          .add(PatternWord("return", "return").setAlias("key"))
          .add(PatternWord("index", "index").setAlias("key"))
          .add(PatternWord("acceptMethods", "accept_methods").setAlias("key"))
          .add(PatternWord("cgiExtension", "cgi_extension").setAlias("key"))

          // simple directive patterns
          .add(PatternSequence("clientMaxContentSizePattern")
                   .add("clientMaxContentSize")
                   .add("words1")
                   .add("simpleEnd")
                   .setAlias("key")
                   .flatResult())
          .add(PatternSequence("rootPattern")
                   .add("root")
                   .add("words1")
                   .add("simpleEnd")
                   .setAlias("key")
                   .flatResult())
          .add(PatternSequence("serverNamePattern")
                   .add("serverName")
                   .add("words1")
                   .add("simpleEnd")
                   .setAlias("key")
                   .flatResult())
          .add(PatternSequence("listenPattern")
                   .add("listen")
                   .add("words1")
                   .add("simpleEnd")
                   .setAlias("key")
                   .flatResult())
          .add(PatternSequence("returnPattern")
                   .add("return")
                   .add("words2")
                   .add("simpleEnd")
                   .setAlias("key")
                   .flatResult())
          .add(PatternSequence("indexPattern")
                   .add("index")
                   .add("words1")
                   .add("simpleEnd")
                   .setAlias("key")
                   .flatResult())
          .add(PatternSequence("acceptMethodsPattern")
                   .add("acceptMethods")
                   .add("words1~")
                   .add("simpleEnd")
                   .setAlias("key")
                   .flatResult())
          .add(PatternSequence("cgiExtensionPattern")
                   .add("cgiExtension")
                   .add("words2")
                   .add("simpleEnd")
                   .setAlias("key")
                   .flatResult())

          // block directive patterns
          .add(PatternSequence("httpBlock")
                   .add("http")
                   .add("blockStart")
                   .add("httpBody")
                   .add("blockEnd")
                   .setAlias("block"))
          .add(PatternOptional("httpBody")
                   .add("rootPattern")
                   .add("clientMaxContentSizePattern")
                   .add("serverBlock")
                   .setAlias("body"))
          .add(PatternSequence("serverBlock")
                   .add("server")
                   .add("blockStart")
                   .add("serverBody")
                   .add("blockEnd")
                   .setAlias("block"))
          .add(PatternOptional("serverBody")
                   .add("rootPattern")
                   .add("clientMaxContentSizePattern")
                   .add("listenPattern")
                   .add("serverNamePattern")
                   .add("locationBlock")
                   .setAlias("body"))
          .add(PatternSequence("locationBlock")
                   .add("locationPath")
                   .add("blockStart")
                   .add("locationBody")
                   .add("blockEnd")
                   .setAlias("block"))
          .add(PatternOptional("locationBody")
                   .add("rootPattern")
                   .add("clientMaxContentSizePattern")
                   .add("returnPattern")
                   .add("indexPattern")
                   .add("acceptMethodsPattern")
                   .add("cgiExtensionPattern")
                   .setAlias("body"))

          // custom
          .add(PatternSequence("locationPath")
                   .add("location")
                   .add("words1")
                   .setAlias("key")
                   .flatResult());

  try {
    ParseResult result = configParser.compile("httpBlock", raw);
    return parseResultToDirective(result);
  } catch (std::invalid_argument &e) {
    std::cout << e.what() << std::endl;
    throw e;
  }
}

const Directive ConfigLexer::parseResultToDirective(const ParseResult &res) {
  Directive dir;
  if (res.getName() == "block") {
    const ParseResult &key = res.getChild(0);
    dir.setKey(key.getValue(0));
    if (key.getValues().size() > 1) {
      std::vector<std::string> tmp;
      std::vector<std::string>::const_iterator it;
      for (it = key.getValues().begin() + 1; it != key.getValues().end();
           it++) {
        tmp.push_back(*it);
      }
      dir.setValues(tmp);
    }
    const ParseResult &body = res.getChild(1);
    const std::vector<ParseResult> &children = body.getChildren();
    for (size_t i = 0; i < children.size(); i++) {
      dir.addChildren(parseResultToDirective(children[i]));
    }
  } else if (res.getName() == "key") {
    dir.setKey(res.getValue(0));
    if (res.getValues().size() > 1) {
      std::vector<std::string> tmp;
      std::vector<std::string>::const_iterator it;
      for (it = res.getValues().begin() + 1; it != res.getValues().end();
           it++) {
        tmp.push_back(*it);
      }
      dir.setValues(tmp);
    }
  }
  return dir;
}
