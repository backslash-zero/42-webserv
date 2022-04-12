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
	(void)path;
	DIR *dir = opendir(path.c_str());
	if (dir == NULL){
		setError(404);
		return 0;
	}
	_body << "<html>\n<head><title>" << _req.getPath() <<"</title></head>\n<body bgcolor=\"white\">\n<center>";
    for (struct dirent *dirEntry = readdir(dir); dirEntry; dirEntry = readdir(dir)) {
        if (std::string(dirEntry->d_name) != ".." && path != "/")
			_body <<"<p><a href=\"http://" << _currentConf.listen << _req.getPath() << "/" << dirEntry->d_name << "\">" << dirEntry->d_name << "</a></p>" << std::endl;
    }
	_body << "</center>\n<hr><center><h3>WebServer</h3></center>\n</body>\n</html>";
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
	else if (false){//(_currentLoc.autoindex == "on" || (_currentConf.autoindex == "on" && _currentLoc.autoindex != "off") ){ // auto indexing
		if (autoIndex(_currentConf.root + _req.getPath())){
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