#include "../incs/cgi.hpp"

cgi::cgi() {
	_env.clear();
	_env.push_back("SERVER_NAME=webserv");
	_env.push_back("SERVER_SOFTWARE=nginx/1.21.5");
	_env.push_back("SERVER_PROTOCOL=HTTP/1.1");
	_env.push_back("SERVER_PORT=8080");
	_env.push_back("GATEWAY_INTERFACE=CGI/1.1");

	_env.push_back("REQUEST_METHOD=POST");
	_env.push_back("PATH_INFO=/index.html");
	_env.push_back("PATH_TRANSLATED=");
	_env.push_back("SCRIPT_NAME=./test/ubuntu_cgi_tester");
	_env.push_back("SCRIPT_FILENAME=./test/ubuntu_cgi_tester");
	_env.push_back("QUERY_STRING=");
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
	_env.push_back("REDIRECT_STATUS=200");
	// tmp dir where the files will be uploaded
	_env.push_back("TMP_DIR=");
	_envTab[NB_CGI_VAR] = 0;
}

void	cgi::convertToC(void) {
	int i = 0;

	for (std::vector<std::string>::iterator it = _env.begin(); it != _env.end(); it++) {
		_envTab[i] = (char*)_env[i].c_str();
		_envTab[i][(_env[i].size())] = '\0';
		i++;
	}
}

void	cgi::exec_child(pid_t pid, int read_fd[2], int write_fd[2], std::string exec) {
	std::cout << "\n[exec child]" << std::endl;
	char *argv[3];
	argv[0] = strdup(exec.c_str());
	argv[1] = strdup(_env[cgi::SCRIPT_FILENAME].c_str());
	argv[2] = NULL;
	// close(write_fd[1]);
	// close(read_fd[0]);
	if (dup2(read_fd[1], STDOUT_FILENO) < 0) {
		std::cout << "error : dup2 read failure" << std::endl;
		close(write_fd[0]);
		close(read_fd[1]);
		return ;
	}
	if (dup2(write_fd[0], STDIN_FILENO) < 0) {
		std::cout << "error : dup2 write failure" << std::endl;
		close(write_fd[0]);
		close(read_fd[1]);
		return ;
	}
	int ret;
	if ((ret = execve(argv[0], argv, _envTab)) < 0) {
		std::cout << "error : execve failure, error: " << errno<< std::endl;
		close(write_fd[0]);
		close(read_fd[1]);
		kill(pid, SIGTERM);
	}
	std::cout << "HERE" << std::endl;
	char	buffer[100] = {0};
	ret = 1;
	while (ret > 0)
	{
		memset(buffer, 0, 100);
		ret = read(write_fd[0], buffer, 100 - 1);
		std::cout << buffer << std::endl;
	}
	std::cout << read_fd[1] << std::endl;
	free(argv[0]);
	free(argv[1]);
}
