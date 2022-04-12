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

int		Response::readFile(std::string path){
	std::ifstream file;
	if (isFile(path)){ // file exist
		file.open(path.c_str());
		_body << file.rdbuf();
		file.close();
	}
	else if (false){ // auto indexing

		return 0;
	}
	else { //file doesn't exist
		setError(404);
		return 0;
	}
	return 1;
}

void	Response::getMethod(){
	if (false) { //check for cgi
		// execute cgi
	}
	else if (readFile(_currentPath)) {
		setContentType(_currentPath);
		setupHeader();
	}
	return ;
}