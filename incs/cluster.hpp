#ifndef CONFIG_CONTEXT_HPP
#define CONFIG_CONTEXT_HPP

#include "../incs/server.hpp"

struct s_location {
	std::string							path;
	std::string							root;
	std::list<std::string>				index;
	std::string							autoindex;
	std::vector<std::string>			methods;
	std::map<std::string, std::string>	fastcgi_param;
	std::map<std::string, std::string>	fastcgi_pass;
};

struct s_server_config {
	std::string							listen;
	std::list<std::string>				server_name;
	std::map<std::string, std::string>	error_page;
	std::string							client_max_body_size;
	std::string							root;
	std::list<std::string>				index;
	std::string							autoindex;
	std::vector<s_location>				location;
};

class Cluster {

	public:
		Cluster();

		void	exploitTokens(std::vector<std::string> &tokens);
		bool	initCluster();
		bool	launch();
		void	printConfig(void);

	private:
	std::vector<s_server_config>	_serverConf;
	std::map<int, Server *> _servers; // stores all servers created
	std::map<int, Server *> _clients; // stores all clients connected to serv

	int		_max_sk;
	fd_set	_msfd; //master fd, handling incoming connection
	std::vector<s_location>	_setupLocation(std::vector<std::string> &loc);
	s_server_config			_setupServer(std::vector<std::string> &serv, std::vector<std::string> &loc);
	void					_clearLocation(s_location lc);
  
	// Exceptions:
	// 1 - File not found
	// class fileNotFoundException : public std::exception
	// {
	// public:
	// 	virtual const char *what() const throw()
	// 	{
	// 		return ("File not foud");
	// 	}
	// }
	// // 2 - Problem with configuration
	// class invalidArgumentException : public std::exception
	// {
	// public:
	// 	virtual const char *what() const throw()
	// 	{
	// 		return ("Invalid configuration file");
	// 	}
	// };
};

#endif
