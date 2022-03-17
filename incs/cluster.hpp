#pragma once

#include "server.hpp"

class Cluster
{
public:
	// Constructors
	Cluster(void);
	~Cluster(void);

	void parseConf(char *conf_file);
	bool initCluster(void);

private:
	// Members:
	std::map<int, Server> _servers; // stores all servers created

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