#ifndef CGI_HPP
# define CGI_HPP

#include <vector>
#include <string>

class cgi {

	public :

	enum index {
		SERVER_SOFTWARE,
		SERVER_NAME,
		GATEWAY_INTERFACE,
		SERVER_PROTOCOL,
		SERVER_PORT,
		REQUEST_METHOD,
		PATH_INFO,
		PATH_TRANSLATED,
		SCRIPT_NAME,
		QUERY_STRING,
		REMOTE_HOST,
		REMOTE_ADDR,
		AUTH_TYPE,
		REMOTE_USER,
		REMOTE_IDENT,
		CONTENT_TYPE,
		CONTENT_LENGTH,
		HTTP_ACCEPT,
		HTTP_ACCEPT_LANGUAGE,
		HTTP_USER_AGENT,
		HTTP_COOKIE,
		HTTP_REFERER,
		REDIRECT_STATUS,
		SCRIPT_FILENAME,
		TMP_DIR,
		NB_CGI_VAR,
	};
	
	cgi();

	void	convertToC(void);

	std::vector<std::string>	_env;
	char						*_envTab[NB_CGI_VAR + 1];
};

#endif
