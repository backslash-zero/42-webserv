#include "../incs/cgi.hpp"
#include "../incs/response.hpp"

cgi::cgi() {
}

void	cgi::setupEnv(Response *resp) {
	(void)resp;
	std::cout << resp->_req.getBody()<<std::endl;
	_env.clear();
	_env.push_back("SERVER_NAME=webserv");
	_env.push_back("SERVER_SOFTWARE=nginx/1.21.5");
	_env.push_back("SERVER_PROTOCOL=HTTP/1.1");
	_env.push_back("SERVER_PORT=" + resp->_serv->getPort());

	_env.push_back("GATEWAY_INTERFACE=CGI/1.1");

	_env.push_back("REQUEST_METHOD=" + resp->_req.getMethod());
	_env.push_back("PATH_INFO=" + resp->_req.getPath());
	_env.push_back("SCRIPT_FILENAME=" + resp->_req.getPath());
	_env.push_back("REQUEST_URI=" + resp->_req.getPath());
	_env.push_back("SCRIPT_NAME=" + resp->_currentLoc.fastcgi_pass);
	_env.push_back("REDIRECT_STATUS=200");

	//_env.push_back("PATH_TRANSLATED=");
	/*_env.push_back("QUERY_STRING=");
	_env.push_back("REMOTE_HOST=");
	_env.push_back("REMOTE_ADDR=");
	_env.push_back("AUTH_TYPE=");
	_env.push_back("REMOTE_USER=");
	_env.push_back("REMOTE_IDENT=");
	_env.push_back("CONTENT_TYPE=");
	_env.push_back("CONTENT_LENGTH=");
	_env.push_back("HTTP_ACCEPT=");
	_env.push_back("HTTP_ACCEPT_LANGUAGE=");
	_env.push_back("HTTP_USER_AGENT=");
	_env.push_back("HTTP_COOKIE=");
	_env.push_back("HTTP_REFERER=");
	// security of php-cgi redirect status 200
	// tmp dir where the files will be uploaded
	_env.push_back("TMP_DIR=");
	_envTab[NB_CGI_VAR] = 0;*/
}

void	cgi::convertToC(Response *resp) {
	int i = 0;
	setupEnv(resp);
	for (std::vector<std::string>::iterator it = _env.begin(); it != _env.end(); it++) {
		_envTab[i] = (char*)_env[i].c_str();
		_envTab[i][(_env[i].size())] = '\0';
		i++;
	}
}

void	cgi::exec_child(std::string exec) {
	pid_t	pid;
	int		saveIn;
	int		saveOut;
	std::string resp;

	saveIn = dup(STDIN_FILENO);
	saveOut = dup(STDOUT_FILENO);
	FILE	*fileIn = tmpfile();
	FILE	*fileOut = tmpfile();
	long	fdIn = fileno(fileIn);
	long	fdOut = fileno(fileOut);
	int		ret = 1;

	//test
	std::string _body = "lol";
	write(fdIn, _body.c_str(), _body.size());
	lseek(fdIn, 0, SEEK_SET);
	if ((pid = fork()) == -1) {
		std::cout << "error fork" << std::endl;
		return ;
	}
	if (!pid) {
		char * const * nll = NULL;
		if (dup2(fdOut, STDOUT_FILENO) == -1) {
			std::cout << "error : dup2 read failure" << std::endl;
			return ;
		}
		if (dup2(fdIn, STDIN_FILENO) == -1) {
			std::cout << "error : dup2 write failure" << std::endl;
			return ;
		}
		if ((ret = execve(exec.c_str(), nll, _envTab)) == -1) {
			std::cout << "error : execve failure, error: " << errno<< std::endl;
			kill(pid, SIGTERM);
		}
	}
	else {
		char	buffer[100] = {0};

		if (waitpid(-1, NULL, 0) == -1)
			return ;
		lseek(fdOut, 0, SEEK_SET);
		ret = 1;
		while (ret > 0) {
			memset(buffer, 0, 100);
			ret = read(fdOut, buffer, 100 - 1);
			resp += buffer;
		}
	}
	dup2(saveIn, STDIN_FILENO);
	dup2(saveOut, STDOUT_FILENO);
	fclose(fileIn);
	fclose(fileOut);
	close(fdIn);
	close(fdOut);
	close(saveIn);
	close(saveOut);

	std::cout << resp << std::endl;
}
