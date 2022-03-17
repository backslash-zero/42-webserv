#include "incs/webserv.hpp"

int launch_cluster(char *conf_file)
{
	Cluster cluster;

	// Parsing .conf
	try
	{
		cluster.parseConf(conf_file);
	}
	catch (const std::exception &e)
	{
		printError(e.what());
		return (1);
	}

	// Initializing cluster

	if (!cluster.initCluster()) // .conf parsing
		return (1);

	return (0);
}

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		printBoldError("Invalid number of arguments");
		printTip("Usage: ./webserv config_file.conf");
		return (1);
	}
	return (launch_cluster(argv[1]));
}