#pragma once

#include "../incs/server.hpp"

class Cluster {

	public:

	typedef std::map<int, std::vector<s_server_config> > t_conf;

	Cluster();

	void	exploitTokens(std::vector<std::string> &tokens);
	bool	initCluster();
	bool	launch();
	void	check_port_server_name(void);
	void	printConfig(void);

	private:
	
	t_conf 							_config; // conf for each port
	std::vector<s_server_config>	_serverConf; // raw conf for each server
	std::map<int, Server *>			_servers; // stores all servers created
	std::map<int, Server *>			_clients; // stores all clients connected to serv

	int								_max_sk;
	fd_set							_msfd; //master fd, handling incoming connection
	std::vector<s_location>			_setupLocation(std::vector<std::string> &loc);
	s_server_config					_setupServer(std::vector<std::string> &serv, std::vector<std::string> &loc);
	void							_clearLocation(s_location lc);
	t_conf							_getListen();
};
