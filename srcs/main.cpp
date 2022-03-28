#include "../incs/webserv.hpp"
#include "../incs/conf_parser.hpp"
#include "../incs/cluster.hpp"
#include "../incs/error.hpp"
int launch_cluster(char *conf_file)
{
	Parser			parser;
	Cluster			cluster;

	std::vector<std::string> tokens = parser.ParseFile(conf_file);
	cluster.exploitTokens(tokens);
	cluster.initCluster();
	cluster.launch();
	//config.printConfig();
	// // Parsing .conf
	// try
	// {
	// 	// Parsing with conf_parser.h: returns vector<Config>
	// }
	// catch (const std::exception &e)
	// {
	// 	printError(e.what());
	// 	return (1);
	// }
	// // Initializing cluster

	// if (!cluster.initCluster()) // .conf parsing
	// 	return (1);

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