#pragma once

#include "server.hpp"

class Cluster
{
public:
	Cluster(void);
	~Cluster(void);

private:
	std::map<int, Server> _servers;
};