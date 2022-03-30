#pragma once

#include "webserv.hpp"

class Server{

	public:

	Server(int port);
	~Server();
	int		setup();
	int		accept();
	bool 	listenClient(int client_fd);

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
};
