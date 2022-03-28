#ifndef CONFIG_CONTEXT_HPP
#define CONFIG_CONTEXT_HPP

#include <string>
#include <vector>
#include <list>
#include <map>
#include <cstdlib>

struct s_location {
	std::string							root;
	std::list<std::string>				index;
	std::string							autoindex;
	std::map<std::string, std::string>	fastcgi_param;
	std::map<std::string, std::string>	fastcgi_pass;
};

struct s_server {
	std::string					listen;
	std::list<std::string>		server_name;
	std::map<int, std::string>	error_page;
	int							client_max_body_size;
	std::string					root;
	std::list<std::string>		index;
	std::string					autoindex;
	std::vector<s_location>		location;
};

class configContext {

	public:

	void	exploitTokens(std::vector<std::string> &tokens);
	void	printConfig(void);

	private:

	std::vector<s_server>	_server;

	std::vector<s_location>	_setupLocation(std::vector<std::string> &loc);
	s_server				_setupServer(std::vector<std::string> &serv, std::vector<std::string> &loc);
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
