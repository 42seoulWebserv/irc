#include "Directive.hpp"

RootConfig RealConfigParser::somefunction() {
  std::string configStr = FileReader::read(filepath);
  ConfigLexer lexer(configStr);
  if (lexer.fail())
    return NULL;

  BlockDirective rootDirective = lexer.getRootRedirective();

  ConfigParser parser(rootDirective);
  if (parser.fail())
    return NULL;

  RootConfig config = parser.getRootConfig();

  return config;
}

int main() {
  RealConfigParser parser("path");

  if (parser.fail())
    return 1;

  RootConfig config = parser.somefunction();

  -- -- -- --
}

// Directive, Config, RealConfigParser -> junmkang
// ConfigLexer -> yonshin
// ConfigParse -> cheseo

// int main()
// {
// 	BlockDirective server1;
// 	server1.key = "server";
// 	server1.children.push_back(SimpleDirective("port", "8080"));
// 	server1.children.push_back(SimpleDirective("server_name", "localhost"));

// 	BlockDirective server2;
// 	server2.key = "server";
// 	server2.children.push_back(SimpleDirective("port", "8080"));
// 	server2.children.push_back(SimpleDirective("server_name", "localhost"));

// 	BlockDirective server3;
// 	server3.key = "server";
// 	server3.children.push_back(SimpleDirective("port", "8080"));
// 	server3.children.push_back(SimpleDirective("server_name", "localhost"));

// 	BlockDirective http;
// 	http.key = "http";
// 	http.children.push_back(BlockDirective(server1));
// 	http.children.push_back(BlockDirective(server2));
// 	http.children.push_back(BlockDirective(server3));

// 	BlockDirective directive;
// 	directive.key = "root";
// 	directive.children.push_back(SimpleDirective("client_max_content_size",
// "50m")); 	directive.children.push_back(BlockDirective(http));
// }
