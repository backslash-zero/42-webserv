#pragma once

#include <string>

#include "server.hpp"
#include "request.hpp"

#define NEWLINE "\r\n"

class Response
{
public:
	Response(Request *request, Request *server);
	~Response(void);

	// Main function
	std::string getResponse(void);

private:
	// Methods responses
	std::string getMethodResponse(void);
	std::string headMethodResponse(void);
	std::string postMethodResponse(void);
	std::string putMethodResponse(void);
	std::string deleteMethodResponse(void);

	//

	// Members
	Request *_request;
	Request *_server;

	std::string _code;
	ResponseHeader responseHeader;
	std::string _body;
};

class ResponseHeader
{
public:
	ResponseHeader(void);
	~ResponseHeader(void);

	std::string getResponseHeader(const Request &request, const Server &server);

private:
};