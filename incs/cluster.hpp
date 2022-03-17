#pragma once

#include "server.hpp"

class Cluster
{
public:
	// Constructors
	Cluster(void);
	~Cluster(void);

	void getConfig(char *conf_file);

private:
	// Members:
	std::map<int, Server> _servers; // stores all servers created

	// Parsing .conf private functions:

	// Exceptions:
	// 1 - File not found
	// 2 - Problem with configuration
};