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
		Request			_req;
		Server*			_serv;
		s_server_config _currentConf;
		int				_ret;

		std::stringstream		_response;
		std::stringstream		_header;
		std::stringstream 		_body;
    	std::map <std::string, std::string> 	_headerTemplate;
		std::map<int, std::string>		_errorTemplate;

		typedef void (Response::*f)();
		std::map <std::string, f> _methodFt;


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

		std::string		getHtmlFile(const std::string& path);

		int		readFile(std::string path);
		int		isFile(std::string path);
};
