#include "ConfigChecker.hpp"
#include "ConfigMaker.hpp"
#include "Directive.hpp"

#include "ConfigLexer.hpp"

int main() {

  std::string file;
  file += "http {";
  // file += "    root /www;";
  file += "    server {";
  // file += "        server_name test;";
  // file += "        client_max_content_size 50m;";
  file += "        listen 8080;";
  // file += "        server_name localhost;";
  // file += "        client_max_content_size 50m;";
  file += "        location / {";
  file += "            return 301 /www/error.html;";
  // file += "            index index.html;";
  file += "            accept_methods POST GET DELETE;";
  file += "            cgi_extension .php /bin/php;";
  file += "            cgi_extension .py /bin/python3;";
  file += "        }";
  file += "    }";
  file += "    server {";
  file += "        listen 8080;";
  file += "        server_name localhost;";
  file += "    }";
  file += "    server {";
  file += "        listen 8080;";
  file += "        server_name localhost;";
  file += "    }";
  file += "}";

  Directive directive = ConfigLexer::run(file);

  // Directive http("http");
  // // Directive root("root");
  // // root.values.push_back("/www");
  // // http.children.push_back(root);

  // Directive server("server");

  // // server_name
  // Directive server_name("server_name");
  // server_name.values.push_back("test");
  // server.children.push_back(server_name);

  // // client_max_content_size
  // Directive client_max_content_size("client_max_content_size");
  // client_max_content_size.values.push_back("50m");
  // server.children.push_back(client_max_content_size);

  // // listen
  // Directive listen("listen");
  // listen.values.push_back("8080");
  // server.children.push_back(listen);

  // // location
  // Directive location("location");
  // location.values.push_back("/");

  // Directive location_return("return");
  // location_return.values.push_back("301");
  // location_return.values.push_back("/www/error.html");
  // location.children.push_back(location_return);

  // Directive location_index("index");
  // location_index.values.push_back("index.html");
  // location.children.push_back(location_index);

  // Directive accept("accept_methods");
  // accept.values.push_back("POST");
  // accept.values.push_back("GET");
  // accept.values.push_back("DELETE");
  // location.children.push_back(accept);

  // Directive cgi1("cgi_extension");
  // cgi1.values.push_back(".php");
  // cgi1.values.push_back("/bin/php");
  // location.children.push_back(cgi1);

  // Directive cgi2("cgi_extension");
  // cgi2.values.push_back(".py");
  // cgi2.values.push_back("/bin/python3");
  // location.children.push_back(cgi2);

  // server.children.push_back(location);

  // http.children.push_back(server);

  Checker::checkDirective(directive);
  RootConfig res = ConfigMaker::makeConfig(directive);
  res.printRootConfig();
}
