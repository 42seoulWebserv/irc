#ifndef Config_HPP
#define Config_HPP

#include <iostream>
#include <map>
#include <vector>

class RootConfig
{
	std::vector<ServerConfig> serverConfigs;
};

class ServerConfig
{
	int port;
	int limitClientBodySize;
	std::string rootPath;
	std::string serverName;
	std::map<int, std::string> errorPages;

	std::vector<LocationConfig> locationConfigs;
};

class LocationConfig
{
	std::string uri;
	std::vector<std::string> acceptMethods;
	std::string rootPath;

	int redirectionStatusCode;
	std::string redirectionPath;

	bool autoIndex; // default false
	std::string indexFile;
	
	std::map<std::string, std::string> cgiPrograms;
};

#endif