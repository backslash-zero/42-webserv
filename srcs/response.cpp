#include "../incs/response.hpp"

template<class T>
std::string toString(const T &value) {
    std::ostringstream os;
    os << value;
    return os.str();
}

Response::Response(Request &request, Server *server):_req(request), _serv(server){
	setupError();
	setupConf();
	_ret = _req.getRet();

	_methodFt["GET"] = &Response::getMethod;
}

std::string		Response::process(){ // creation of response
	if (_ret == 200){

		(this->*_methodFt[_req.getMethod()])();
	}
	else { // error on request
		setError(_ret);
	}
	writeResp();
	return _response.str();
}

void		Response::setError(int ret){
	_ret = ret;
	_headerTemplate["Content-Type"] = "text/html";
	_headerTemplate["Connection"] = "Close";
	std::map<std::string, std::string>::iterator errPage = _currentConf.error_page.find(toString(_ret));
	if (errPage != _currentConf.error_page.end() && isFile(errPage->second)){ // err Page is defined in conf file
		_body << getHtmlFile(errPage->second);
	}
	else{ //default error page
		_body << getHtmlFile("html/" + toString(_ret) + ".html");
	}
	_headerTemplate["Content-Length"] = setContentLength();

}

void		Response::writeResp(){
	_header << "HTTP/1.1 " << _ret << " " << _errorTemplate[_ret] << "\r\n";
	std::map<std::string, std::string>::iterator it = _headerTemplate.begin();
	for ( ; it != _headerTemplate.end(); it++){
		_header << it->first << ": " << it->second << "\r\n";
	}
	_response << _header.str()<< "\r\n" << _body.str() << "\r\n\r\n";
}

void		Response::setupHeader(){ // setup header map, with corresponding function
	_headerTemplate["Server"] = "webserv/1.0.0 (Unix)";
	_headerTemplate["Connection"] = setConnection();
	_headerTemplate["Date"] = setDate();
	//_headerTemplate["Content-Type"] = "";
	_headerTemplate["Content-Length"] = setContentLength();
}

void		Response::setupError(){ // setup error code with corresponding message
	_errorTemplate[200] = "Ok";
	_errorTemplate[400] = "Bad Request";
	_errorTemplate[403] = "Forbidden";
	_errorTemplate[404] = "Not Found";
	_errorTemplate[405] = "Method Not Allowed";
	_errorTemplate[418] = "I'm a teapot";
	_errorTemplate[500] = "Internal Server Error";
}

Response::~Response(){}

std::string		Response::setConnection(){
	if (_req.getRet() >= 400)
		return "Close";
	else
		return "Keep-Alive";
}

std::string		Response::setServer(){
	return "webserv/1.0.0 (Unix)";
}

void		Response::setupConf(){
	std::string host = _req.getHeaders().find("Host")->second;
	std::vector<s_server_config> _conf = _serv->getConf();
	std::vector<s_server_config>::iterator it = _conf.begin();

	for ( ; it != _conf.end(); it++){
		if (it->server_name.front() == host){
			_currentConf = *it;
			break;
		}
	}
	if (it == _conf.end())
		_currentConf = *_conf.begin();
	_currentPath = _currentConf.root + _req.getPath();
	std::vector<s_location>::iterator ite = _currentConf.location.begin();
	for ( ; ite != _currentConf.location.end(); ite++){
		if (ite->path == _req.getPath()) {
			_currentLoc = *ite;
			if (_currentConf.autoindex != "on" && _currentLoc.autoindex != "on"){
				_currentPath = (_currentLoc.root.size() > 0 ? _currentLoc.root : _currentConf.root) + "/" +
							(_currentLoc.index.size() > 0 ? _currentLoc.index.front() : _currentConf.index.front());
			}

		}
	}
}

std::string		Response::setDate(){
	char			buffer[100];
	struct timeval	tv;
	struct tm		*tm;

	gettimeofday(&tv, NULL);
	tm = gmtime(&tv.tv_sec);
	strftime(buffer, 100, "%a, %d %b %Y %H:%M:%S GMT", tm);
	return std::string(buffer);
}

std::string		Response::setContentLength(){
	return toString(_body.str().size());
}

std::string		Response::setContentType(std::string path)
{
	std::string type = path.substr(path.rfind(".") + 1, path.size() - path.rfind("."));
	if (type == "html")
		return "text/html";
	else if (type == "css")
		return "text/css";
	else if (type == "js")
		return "text/javascript";
	else if (type == "jpeg" || type == "jpg")
		return "image/jpeg";
	else if (type == "png")
		return "image/png";
	else if (type == "bmp")
		return "image/bmp";
	else
		return "text/plain";
}

std::string		Response::getHtmlFile(const std::string& path)
{
	std::ofstream		file;
	std::stringstream	buffer;

	file.open(path.c_str(), std::ifstream::in);
	buffer << file.rdbuf();
	file.close();
	return (buffer.str());
}