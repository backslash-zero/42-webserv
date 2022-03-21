#pragma once

#include "server.hpp"

class Cluster
{
public:
	// Constructors
	Cluster(void);
	~Cluster(void);

	void parseConf(char *conf_file); // parsing .conf and creating configuration
	bool initCluster(void);

	// Config stored as a class?

private:
	// Members:
	std::map<int, Server> _servers; // stores all servers created
	size_t _server_count;
	// Parsing .conf private functions:

	// Exceptions:
	// 1 - File not found
	class fileNotFoundException : public std::exception
	{
	public:
		virtual const char *what() const throw()
		{
			return ("File not foud");
		}
	}
	// 2 - Problem with configuration
	class invalidArgumentException : public std::exception
	{
	public:
		virtual const char *what() const throw()
		{
			return ("Invalid configuration file");
		}
	};
};