#include "Directive.hpp"

int main()
{
	BlockDirective server1;
	server1.key = "server";
	server1.children.push_back(SimpleDirective("port", "8080"));
	server1.children.push_back(SimpleDirective("server_name", "localhost"));

	BlockDirective server2;
	server2.key = "server";
	server2.children.push_back(SimpleDirective("port", "8080"));
	server2.children.push_back(SimpleDirective("server_name", "localhost"));

	BlockDirective server3;
	server3.key = "server";
	server3.children.push_back(SimpleDirective("port", "8080"));
	server3.children.push_back(SimpleDirective("server_name", "localhost"));

	BlockDirective http;
	http.key = "http";
	http.children.push_back(BlockDirective(server1));
	http.children.push_back(BlockDirective(server2));
	http.children.push_back(BlockDirective(server3));

	BlockDirective directive;
	directive.key = "root";
	directive.children.push_back(SimpleDirective("client_max_content_size", "50m"));
	directive.children.push_back(BlockDirective(http));
}
