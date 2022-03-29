
#include <string>
#include <iostream>
#include <fstream>

#include "../incs/response.hpp"

#include "../incs/server.hpp"
#include "../incs/request.hpp"

bool Response::_getFile(const std::string &error_code)
{
}

bool Response::_create_error(const std::string &error_code)
{
	this->_code = error_code;

	// check if corresponding error page is defined
	std::map<std::string, std::string>::iterator it = server->error_page.begin();
	std::map<std::string, std::string>::iterator it = server->error_page.end();

	// Assign default error page

	// Check if path is defined for error
	while (it != end)
	{
		if (it->first == error_code)
			return (true);
		it++;
	}
}

bool Response::_check_method(void)
{
	// !todo: Server needs to extract methods as a vector of strings
	std::vector<s_server>::iterator it = _server->methods.begin();
	std::vector<s_server>::iterator end = _server->methods.end();
	while (it != end)
	{
		if (*it == _request->method)
			return (true);
		it++;
	}
	return (false);
}

Response::Response(Request *request, Request *server) : _request(request), _server(server)
{
	// 1. Check request and server configuration
	if (this->_check_method())
	{
		// 405 error : method not allowed
		_create_error("405");
	}
	else
	{
	}
}

std::string Response::getResponse(const Request &request)
{
	std::string response;
	ResponseHeader responseHeader;

	// X. Execute CGI

	// 2. Launch methodResponse

	response = responseHeader.getResponseHeader(request, server);
}