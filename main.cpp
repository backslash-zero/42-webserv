#include "incs/webserv.hpp"

int launch_cluster(char *conf_file)
{
	Cluster cluster;
	try
	{
		cluster.getConfig(conf_file); // .conf parsing
									  /* code */
	}
	catch (const std::exception &e)
	{
		printError(e.what());
		return (1);
	}
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