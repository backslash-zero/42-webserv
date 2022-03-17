#pragma once

#include <string>
#include <iostream>
#include <map>
#include <sys/socket.h>

#include "error.hpp"

class Server
{
public:
	// Constructors
	Server();

	// Destructor
	Server();

	// Tools
	bool init(void);

private:
	int _server_fd;				 // stores value of socket() function.
	struct sockaddr_in _address; // stores transport address.

	std::map<int, std::string> _requests; // stores requests.
};

bool Server::init(void)
{
	// Creating socket
	this->_server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_server_fd == -1)
	{
		printError("Server could not be created: socket() function error");
		return (false);
	}
	else
	{
		// Assigning a transport address to the socket
		memset((char *)&(this->_address), 0, sizeof(this->_address));
		this->_address.sin_family = AF_INET;
		this->_address.sin_addr.s_addr = htonl(INADDR_ANY); // How to assign port
		this->_address.sin_port = htons(PORT);				// How to assign port
	}
	return (true);
}