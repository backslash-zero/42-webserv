
#include "../incs/server.hpp"

Server::Server(int port):_port(port){

}

int 	Server::setup(){
	_sockfd = socket(AF_INET, SOCK_STREAM, 0);
	// config socket
	if (_sockfd == -1) {
		std::cerr << "Failed to create socket. errno: " << errno << std::endl;
		return -1;
	}
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	sockaddr.sin_port = htons(_port);
	if (::bind(_sockfd, (struct sockaddr*)&sockaddr, sizeof(sockaddr)) < 0){
		std::cerr << "Bind failed " << _port << std::endl;
		close(_sockfd);
		return -1;
	}
	// Start listening. Hold at most 1024 connections in the queue
	if (::listen(_sockfd, 1024) < 0){
		std::cerr << "Listen failed" << std::endl;
		close(_sockfd);
		return -1;
	};
	std::cout << GREEN << "Server listenning on port " << _port << WHITE << std::endl;
	return 0;
}

int		Server::accept(){
	int connection;
	size_t addrlen = sizeof(sockaddr);

	connection = ::accept(_sockfd, (struct sockaddr*)&sockaddr, (socklen_t*)&addrlen); // accept incoming connection, and create specific socket for client (connection)
	if (connection < 0) {
		std::cerr << "Failed to grab connection. errno: " << errno << std::endl;
		close(connection);
	}
	return connection;
}

bool 	Server::listenClient(int client_fd){
	int ret;
	char buffer[8192] = {0};
	ret = ::recv(client_fd, buffer, 8192 - 1, 0); //listen to client
	if (ret <= 0)
		return ret;
	_requests[client_fd] += buffer; //add content to client's request
	std::cout << std::endl << GREEN << "Client on fd " << client_fd << " send \n[" << _requests[client_fd] << "]" << WHITE << std::endl;
	if (_requests[client_fd].find("\r\n\r\n") != std::string::npos){ // if end of request
		// process it
		//Request req(_requests[client_fd]);
		//send response
		::send(client_fd, "HTTP/1.1 200 OK", 16, 0); //exemple
		_requests[client_fd].clear(); //clear request
	}
	return ret;
}

Server::~Server(){
	if (_sockfd){
		close(_sockfd);
		std::cout << "Server shutdown." << std::endl;
	}
}

int 	Server::getPort(){
	return _port;
}
int		Server::getSocket(){
	return _sockfd;
}
sockaddr_in 	Server::getSockaddr(){
	return sockaddr;
}

/*std::string		getHtmlFile(const std::string& path)
{
	std::ofstream		file;
	std::stringstream	buffer;

	file.open(path.c_str(), std::ifstream::in);
	buffer << file.rdbuf();
	file.close();
	return (buffer.str());
}

//exemple
std::string createResponse(int code, std::string param){
	
	std::stringstream ss;
	// setup header
	ss << "HTTP/1.1" << " " << code << std::endl;
	ss << "Server: " << "localhost:" << _port << std::endl;

	if (param.size() >= 0){
		ss << "Content-Type: " << "text/html\n"; // Static need to be modified
		ss << "Content-Length: " << getHtmlFile(param).size() << std::endl;
	}
	ss << "Connection: " << "keep-alive\n";  // Static need to be modified
	
	//setup body
	if (param.size() >=0){
		ss << "\r\n";
		ss << getHtmlFile(param);
		ss << "\r\n\r\n";
	}
	return (ss.str());*/