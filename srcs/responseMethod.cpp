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

void	Response::methodGet(void) {
	if (_currentLoc.fastcgi_pass.size() > 0) { //check for cgi
		cgi		cgi;

		cgi.convertToC(this);
		cgi.exec_child(_currentLoc.fastcgi_pass);
		return ;
	}
	if (readFile(_currentPath)) {
		setContentType(_currentPath);
		setupHeader();
	}
	return ;
}

void	Response::methodDelete(void){
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

void	Response::methodPost(void) {
	if (_currentLoc.fastcgi_pass.size() > 0) { //check for cgi
		cgi		cgi;

		cgi.convertToC(this);
		cgi.exec_child(_currentLoc.fastcgi_pass);
	}
	else {
		writeInFile(_req.getBody());
		if (_ret < 300)
			setupHeader();
	}
	return ;
}

void	Response::methodHead(void) {
	if (readFile(_currentPath)) {
		_body.str("");
		setContentType(_currentPath);
		setupHeader();
	}
	return ;
}

void	Response::writeInFile(std::string body)
{
	std::ofstream	file;
	std::string _path;

	_path = _currentConf.uploads + _currentPath.substr(_currentPath.find_last_of('/'));
	std::cout << _path << std::endl;
	if (isFile(_path))
	{
		file.open(_path.c_str());
		file << body;
		file.close();
		_ret = 204;
	}
	else
	{
		file.open(_path.c_str(), std::ofstream::out | std::ofstream::trunc);
		if (file.is_open() == false){
			setError(403);
			return;
		}

		file << body;
		file.close();
		_ret = 201;
	}
}