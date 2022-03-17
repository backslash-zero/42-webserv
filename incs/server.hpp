#pragma once

#include <string>
#include <iostream>
#include <map>
#include <sys/socket.h>
#include <netinet/in.h>

#include "error.hpp"

#define MAX_CONNECTIONS 1024

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
	if ((this->_server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		printError("Server could not be initialized: ", "socket() call failed");
		return (false);
	}
	// Assigning a transport address to the socket
	// 1. Initializing struct
	memset((char *)&(this->_address), 0, sizeof(this->_address));
	this->_address.sin_family = AF_INET;
	this->_address.sin_addr.s_addr = htonl(INADDR_ANY); // ???How to assign port
	this->_address.sin_port = htons(PORT);				// ???How to assign port
	// 2. Bind call
	if (bind(this->_server_fd, (struct sockaddr *)&(this->_address), sizeof(this->_address)) == -1)
	{
		printError("Server could not be initialized: ", "bind() call failed");
		return (false);
	}
	// 3. Listen call
	if (listen(this->_server_fd, MAX_CONNECTIONS) == -1)
	{
		printError("Server could not be initialized: ", "bind() call failed");
		return (false);
	}
	return (true);
}