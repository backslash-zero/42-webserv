#include <iostream>
#include <iterator>
#include <string.h>
#include <string>

#include "../incs/cluster.hpp"
#include "../incs/cosmetics.hpp"
std::vector<s_location>		Cluster::_setupLocation(std::vector<std::string> &loc) {
	std::vector<s_location>	s_lc;
	s_location				lc;

	std::vector<std::string>::iterator it = loc.begin();
	std::vector<std::string>::iterator ite = loc.end();

	while (it != ite) {
		if (it->compare("root") == 0) {
			it++;
			lc.root = *it;
		}
		if (it->compare("index") == 0) {
			while (it != ite && it->compare(";") != 0) {
				it++;
				if (it->compare(";") != 0)
					lc.index.push_back(*it);
			}
		}
		if (it->compare("autoindex") == 0) {
			it++;
			lc.root = *it;
		}
		if (it->compare("}") == 0) {
			s_lc.push_back(lc);
			lc.root.clear();
			lc.index.clear();
			lc.autoindex.clear();
			lc.fastcgi_param.clear();
			lc.fastcgi_pass.clear();
		}
		it++;
	}
	return s_lc;
}

s_server	Cluster::_setupServer(std::vector<std::string> &serv, std::vector<std::string> &loc) {
	s_server sv;
	std::vector<std::string>::iterator it = serv.begin();
	std::vector<std::string>::iterator ite = serv.end();

	while (it != ite) {
		if (it->compare("listen") == 0) {
			it++;
			sv.listen = *it;
		}
		if (it->compare("server_name") == 0) {
			while (it != ite && it->compare(";") != 0) {
				it++;
				if (it->compare(";") != 0)
					sv.server_name.push_back(*it);
			}
		}
		if (it->compare("error_page") == 0) {
			std::vector<std::string>::iterator tmp = it;
			while (it != ite && it->compare(";") != 0)
				it++;
			it--;
			std::string value = *it;
			serv.erase(it);
			it = tmp;
			while (it != ite && it->compare(";") != 0) {
				it++;
				if (it->compare(";") != 0) {
					std::string tmp = *it;
					sv.error_page.insert(std::make_pair(atoi(tmp.c_str()), value));
				}
			}
		}
		if (it->compare("client_max_body_size") == 0) {
			it++;
			std::string tmp1 = *it;
			sv.client_max_body_size = atoi(tmp1.c_str());
		}
		if (it->compare("root") == 0) {
			it++;
			sv.root = *it;
		}
		if (it->compare("index") == 0) {
			while (it != ite && it->compare(";") != 0) {
				it++;
				if (it->compare(";") != 0)
					sv.index.push_back(*it);
			}
		}
		if (it->compare("autoindex") == 0) {
			it++;
			sv.root = *it;
		}
		it++;
	}
	sv.location = _setupLocation(loc);
	return sv;
}

void	Cluster::exploitTokens(std::vector<std::string> &tokens) {
	std::vector<std::string> vec = tokens;
	std::vector<std::string> loc;
	std::vector<std::string> serv;

	std::vector<std::string>::iterator it = vec.begin();
	std::vector<std::string>::iterator ite = vec.end();
	while (it != ite) {
		if (it->compare("server") == 0) {
			it++;
			if (it->compare("{") != 0)
				std::cout << "error { missing" << std::endl;
			else {
				it--;
				while (it != ite && it->compare("}") != 0) {
					if (it->compare("location") == 0) {
						it++;
						if (it->compare("/") != 0)
							std::cout << "error location_match" << std::endl;
						else {
							it++;
							if (it->compare("{") != 0)
								std::cout << "error { missing" << std::endl;
							it--;
							it--;
							while (it != ite && it->compare("}") != 0) {
								loc.push_back(*it);
								vec.erase(it);
							}
							loc.push_back(*it);
							vec.erase(it);
							ite = vec.end();
							continue ;
						}
					}
					serv.push_back(*it);
					vec.erase(it);
				}
				serv.push_back(*it);
				vec.erase(it);
				_serverConf.push_back(_setupServer(serv, loc));
				serv.clear();
				loc.clear();
				ite = vec.end();
				continue ;
			}
		}
		it++;
	}
}

void	Cluster::printConfig(void) {
	std::vector<s_server>::iterator it = _serverConf.begin();
	std::vector<s_server>::iterator ite = _serverConf.end();
	for ( ; it != ite; it++) {
		std::cout << "[server]" << std::endl;
		if (!it->listen.empty()) {
			std::cout << "listen : " << it->listen << std::endl;
		}
		if (!it->server_name.empty()) {
			for (std::list<std::string>::iterator i = it->server_name.begin(); i != it->server_name.end(); i++)
				std::cout << "server_name : " << *i << std::endl;
		}
		if (!it->error_page.empty()) {
			for (std::map<int, std::string>::iterator i = it->error_page.begin(); i != it->error_page.end(); i++)
				std::cout << "error_page key : " << i->first << " value : " << i->second << std::endl;
		}
		if (it->client_max_body_size) {
			std::cout << "client_max_body_size : " << it->client_max_body_size << std::endl;
		}
		if (!it->root.empty()) {
			std::cout << "root : " << it->root << std::endl;
		}
		if (!it->index.empty()) {
			for (std::list<std::string>::iterator i = it->index.begin(); i != it->index.end(); i++)
				std::cout << "index : " << *i << std::endl;
		}
		if (!it->autoindex.empty())
			std::cout << "autoindex : " << it->autoindex << std::endl;
		if (!it->location.empty()) {
			for (std::vector<s_location>::iterator i = it->location.begin(); i != it->location.end(); i++) {
				std::cout << "[location]" << std::endl;
				if (!i->root.empty()) {
					std::cout << "root : " << i->root << std::endl;
				}
				if (!i->index.empty()) {
					for (std::list<std::string>::iterator j = i->index.begin(); j != i->index.end(); j++)
						std::cout << "index : " << *j << std::endl;
				}
				if (!i->autoindex.empty())
					std::cout << "autoindex : " << i->autoindex << std::endl;
			}
		}
	}
}

bool		Cluster::initCluster(){
	std::vector<s_server>::iterator it = _serverConf.begin();
	std::vector<s_server>::iterator ite = _serverConf.end();
	for ( ; it != ite; it++) {
		// for each port we instatiate server
		int port = atoi(it->listen.c_str());
		_servers.insert(std::make_pair(port, new Server(port)));
		long		fd;
		if (_servers[port]->setup() != -1)
		{
			fd = _servers[port]->getSocket();
			FD_SET(fd, &_msfd); // bind socket (fd) to master fd (_msfd) see select doc.
			if (fd > _max_sk)
				_max_sk = fd;
		}
		else {
			_servers.erase(port); // if failed setup, remove from server list
		}
	}
	return true;
}

bool		Cluster::launch(){
	struct timeval tv;
	int recVal = 0;
	tv.tv_sec = 1;
	while(true)
	{
		fd_set rfds;

		FD_ZERO(&rfds);
		memcpy(&rfds, &_msfd, sizeof(_msfd));
		std::cout << "\rWaiting for client" << std::flush;
		recVal = select(_max_sk + 1, &rfds, NULL, NULL, &tv); // wait for change on rfds
		switch(recVal)
		{
			case(0):{//Timeout
				break;
			}
			case(-1):
			{
				std::cerr << RED << "\tSelect error :" << errno << WHITE << std::flush;
				for (std::map<int, Server *>::iterator it = _servers.begin(); it != _servers.end(); it++){
					close(it->first);
				}
				break ;
			}
			default: //change append
			{
				for (std::map<int, Server *>::iterator it = _clients.begin(); it != _clients.end(); it++){ //iterate on connected clients
					if (FD_ISSET(it->first, &rfds)){ // search the client who send us smth
						int client_fd = it->first;
						if (it->second->listenClient(client_fd) <= 0){ // listen to it
							std::cout << RED << "\nConnection "<< client_fd<< " closed." << WHITE << std::endl;
							close(it->first);
							FD_CLR(it->first, &rfds);
							FD_CLR(it->first, &_msfd);
							_clients.erase(it->first);
							it = _clients.begin();
						}
						recVal = 0;
						break;
					}
				}
				for (std::map<int, Server *>::iterator it = _servers.begin(); it != _servers.end(); it++){ // iterate on server
					if (FD_ISSET(it->second->getSocket(), &rfds)){ // search for the server who send us smth
						int connection = it->second->accept(); // accept incomming connection
						if (connection != -1){
							std::cout << GREEN << "\nClient connected at port : " << it->second->getPort() << WHITE<< std::endl;
							FD_SET(connection, &_msfd); // root client socket (connection) to master fd
							_clients.insert(std::make_pair(connection, it->second)); // add client to connected client (_clients)
							if (connection > _max_sk)
								_max_sk = connection;
						}
					}
				}
				break;
			}
		}
	}
}