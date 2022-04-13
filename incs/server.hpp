#pragma once

#include "webserv.hpp"

struct s_location {
	std::string							path;
	std::string							root;
	std::list<std::string>				index;
	std::string							autoindex;
	std::vector<std::string>			methods;
	std::string							fastcgi_pass;
	// std::map<std::string, std::string>	fastcgi_param;
	std::vector<s_location>				location;
};

struct s_server_config {
	std::string							listen;
	int									port;
	std::list<std::string>				server_name;
	std::map<std::string, std::string>	error_page;
	std::string							client_max_body_size;
	std::string							root;
	std::list<std::string>				index;
	std::string							autoindex;
	std::vector<s_location>				location;
};

class Server{

	public:

		Server(int port, std::vector<s_server_config>);
		~Server();
		int		setup();
		int		accept();
		bool 	listenClient(int client_fd);
		std::vector<s_server_config> getConf();
		int 			getPort();
		int				getSocket();
		sockaddr_in 	getSockaddr();

		std::string 	createResponse(int code, std::string param);
		std::string		getHtmlFile(const std::string& path);

	private:

		int							_port;
		int							_sockfd;
		sockaddr_in					sockaddr;
		std::map<int, std::string>	_requests; // stores requests.
		std::vector<s_server_config> _conf; //conf of server/host
};
