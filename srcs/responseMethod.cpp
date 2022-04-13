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
	std::cout << path << std::endl;
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

void	Response::getMethod(){
	if (_currentLoc.fastcgi_pass.size() > 0) { //check for cgi
		// execute cgi
	}
	else if (readFile(_currentPath)) {
		setContentType(_currentPath);
		setupHeader();
	}
	return ;
}