#include "../incs/response.hpp"

int		Response::isFile(std::string path){
	struct stat s;
	if (stat(path.c_str(), &s) == 0 )
	{
		if (s.st_mode & S_IFDIR)
			return 0;
		else if (s.st_mode & S_IFREG) //is readable file
			return 1;
		else
			return 0;
	}
	else {
		return 0;
	}
}

int		Response::autoIndex(std::string path){
	DIR *dir = opendir(path.c_str());
	if (dir == NULL){
		setError(404);
		return 0;
	}
	_body << "<html>\n<head><title>" << _req.getPath() <<"</title></head>\n<body bgcolor=\"white\">\n";
	for (struct dirent *dirEntry = readdir(dir); dirEntry; dirEntry = readdir(dir)) {
		if (std::string(dirEntry->d_name) != ".")
			_body <<"<p><a href=\"http://" << _currentConf.listen << _req.getPath() << (*_req.getPath().rbegin() == '/' ? "" : "/") << dirEntry->d_name << "\">" << dirEntry->d_name << "</a></p>" << std::endl;
	}
	_body << "\n<hr><center><h3>WebServer</h3></center>\n</body>\n</html>";
	return 1;
}

int		Response::readFile(std::string path){
	std::ifstream file;
	if (isFile(path)){ // file exist
		file.open(path.c_str());
		_body << file.rdbuf();
		file.close();
	}
	else if (_currentLoc.autoindex == "on" || (_currentConf.autoindex == "on" && _currentLoc.autoindex != "off") ){ // auto indexing
		if (autoIndex(path)){
			_headerTemplate["Content-Type"] = "text/html";
			setupHeader();
		}
		return 0;
	}
	else { //file doesn't exist
		setError(404);
		return 0;
	}
	return 1;
}

void	Response::getMethod(void) {
	if (_currentLoc.fastcgi_pass.size() > 0) { //check for cgi
		// execute cgi
	}
	if (readFile(_currentPath)) {
		setContentType(_currentPath);
		setupHeader();
	}
	return ;
}

void	Response::deleteMethod(void){
	if (_currentPath.find("..")) {
		setError(403);
		setupHeader();
		return ;
	}
	if (isFile(_currentPath)){
		if (remove(_currentPath.c_str()) == 0)
			_ret = 204;
		else
			setError(403);
	}
	else
		setError(404);
	setupHeader();
	return ;
}

void	Response::exec_child(pid_t pid, cgi *cgi, std::string exec) {
	std::cout << "exec child" << std::endl;
	char *argv[3];
	argv[0] = strdup(exec.c_str());
	argv[1] = strdup(cgi->_env[cgi::SCRIPT_FILENAME].c_str());
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
	if ((ret = execve(argv[0], argv, cgi->_envTab)) < 0) {
		std::cout << "error : execve failure, error: " << errno<< std::endl;
		close(write_fd[0]);
		close(read_fd[1]);
		kill(pid, SIGTERM);
	}
	free(argv[0]);
	free(argv[1]);
}

void	Response::postMethod(void) {
	if (_currentLoc.fastcgi_pass.size() > 0) { //check for cgi
		cgi		cgi;
		pid_t	pid;

		std::cout << "CGI_PASS => " << _currentLoc.fastcgi_pass << std::endl;
		cgi.convertToC();
		if ((pid = fork()) == -1) {
			std::cout << "error fork" << std::endl;
			return ;
		}
		if (pid == 0) {
			exec_child(pid, &cgi, _currentLoc.fastcgi_pass);
		}
		else {
			close(write_fd[0]);
			close(read_fd[1]);
			waitpid(pid, NULL, WNOHANG);
		}
	}
	else {
		_ret = 204;
		setupHeader();
	}
	return ;

}
