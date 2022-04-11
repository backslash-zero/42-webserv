#pragma once

#include "webserv.hpp"
#include "server.hpp"
#include "request.hpp"

class Response{

	public:
		Response(Request &request, Server *server);
		~Response();

		std::string		process();
	private:
		Request			_req; //store request
		Server*			_serv; // server who received request
		s_server_config _currentConf; // server's conf
		int				_ret; // error code

		std::stringstream		_response; //response which will be send
		std::stringstream		_header; //heade rof resp
		std::stringstream 		_body; //body of resp
    	std::map <std::string, std::string> 	_headerTemplate; // stored header value
		std::map<int, std::string>		_errorTemplate; // errorCode & phrase

		typedef void (Response::*f)();
		std::map <std::string, f> _methodFt; //map of function's method


		void	setupHeader(); //setup headerTemplate variable
		void	setupError(); // setup error code and corresponding phrase
		void	setupConf(); // get config of the virtual server

		void	setError(int ret); // set header & body for specific error code

		void	writeResp(); // write error & body

		void	getMethod();

		std::string		setConnection();
		std::string		setServer();
		std::string		setDate();
		std::string		setContentLength();
		std::string		setContentType(std::string path);

		std::string		getHtmlFile(const std::string& path); // read file and return string

		int		readFile(std::string path);
		int		isFile(std::string path);
};
