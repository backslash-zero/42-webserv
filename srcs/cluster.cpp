
#include "../incs/cluster.hpp"

static s_location		_setupLocationBis(const std::vector<std::string> &loc) {
	s_location				lc;

	std::vector<std::string>::const_iterator it = loc.begin();
	std::vector<std::string>::const_iterator ite = loc.end();

	while (it != ite) {
		std::cout << "it : " << *it << std::endl;
		if (it->compare("location") == 0) {
			it++;
			if (it == ite || it->compare(";") == 0)
				throw std::logic_error("argument not find");
			lc.path = *it;
			it++;
			if (it->compare("{") != 0)
				throw std::logic_error("bracket is missing");
			it--;
		}
		if (it->compare("root") == 0) {
			it++;
			if (it == ite || it->compare(";") == 0)
				throw std::logic_error("argument not find");
			lc.root = *it;
		}
		if (it->compare("index") == 0) {
			while (it != ite && it->compare(";") != 0) {
				it++;
				if (it != ite && it->compare(";") != 0){
					lc.index.push_back(*it);
				}
			}
		}
		if (it->compare("autoindex") == 0) {
			it++;
			if (it == ite || it->compare(";") == 0)
				throw std::logic_error("argument not find");
			lc.autoindex = *it;
		}
		if (it->compare("methods") == 0) {
			while (it != ite && it->compare(";") != 0) {
				it++;
				if (it != ite && it->compare(";") != 0)
					lc.methods.push_back(*it);
			}
		}
		if (it->compare("fastcgi_pass") == 0) {
			it++;
			if (it == ite || it->compare(";") == 0)
				throw std::logic_error("argument not find");
			lc.fastcgi_pass = *it;
		}
		it++;
	}
	return lc;
}

std::vector<s_location>		Cluster::_setupLocation(const std::vector<std::string> &loc) {
	std::vector<s_location>	s_lc;
	s_location				lc;

	std::vector<std::string>::const_iterator it = loc.begin();
	std::vector<std::string>::const_iterator ite = loc.end();
	bool first_loc = false;

	while (it != ite) {
		std::cout << "-->it : " << *it << std::endl;
		if (it->compare("location") == 0 && !first_loc) {
			first_loc = true;
			it++;
			if (it == ite || it->compare(";") == 0)
				throw std::logic_error("argument not find");
			lc.path = *it;
			it++;
			if (it->compare("{") != 0)
				throw std::logic_error("bracket is missing");
			it--;
		}
		if (it->compare("location") == 0 && first_loc) {
			std::cout << "location into loc" << std::endl;
			std::vector<std::string>::const_iterator start = it;
			int open = 1;
			while (it != ite && open > 0) {
				it++;
				if (it != ite && it->compare("}") == 0)
					open--;
				else if (it != ite && it->compare("location") == 0)
					open++;
			}
			lc.location.push_back(_setupLocationBis(std::vector<std::string>(start, ++it)));
			continue ;
		}
		if (it->compare("root") == 0) {
			it++;
			if (it == ite || it->compare(";") == 0)
				throw std::logic_error("argument not find");
			lc.root = *it;
		}
		if (it->compare("index") == 0) {
			while (it != ite && it->compare(";") != 0) {
				it++;
				if (it != ite && it->compare(";") != 0){
					lc.index.push_back(*it);
				}
			}
		}
		if (it->compare("autoindex") == 0) {
			it++;
			if (it == ite || it->compare(";") == 0)
				throw std::logic_error("argument not find");
			lc.autoindex = *it;
		}
		if (it->compare("methods") == 0) {
			while (it != ite && it->compare(";") != 0) {
				it++;
				if (it != ite && it->compare(";") != 0)
					lc.methods.push_back(*it);
			}
		}
		if (it->compare("fastcgi_pass") == 0) {
			it++;
			if (it == ite || it->compare(";") == 0)
				throw std::logic_error("argument not find");
			lc.fastcgi_pass = *it;
		}
		if (it->compare("}") == 0) {
			std::cout << "loc closed" << std::endl;
			s_lc.push_back(lc);
			lc.root.clear();
			lc.index.clear();
			lc.autoindex.clear();
			lc.fastcgi_pass.clear();
			lc.location.clear();
			lc.methods.clear();
			first_loc = false;
			// lc.fastcgi_param.clear();
		}
		it++;
	}
	return s_lc;
}

s_server_config	Cluster::_setupServer(std::vector<std::string> &serv, std::vector<std::string> &loc) {
	s_server_config sv;
	std::vector<std::string>::iterator it = serv.begin();
	std::vector<std::string>::iterator ite = serv.end();

	while (it != ite) {
		if (it->compare("listen") == 0) {
			it++;
			if (it == ite || it->compare(";") == 0)
				throw std::logic_error("argument not find");
			sv.listen = *it;
		}
		if (it->compare("server_name") == 0) {
			while (it != ite && it->compare(";") != 0) {
				it++;
				if (it != ite && it->compare(";") != 0)
					sv.server_name.push_back(*it);
			}
		}
		if (it->compare("error_page") == 0) {
			// std::vector<std::string>::iterator tmp = it;
			// while (it != ite && it->compare(";") != 0)
			// 	it++;
			// it--;
			// std::string value = *it;
			// serv.erase(it);
			// it = tmp;
			// while (it != ite && it->compare(";") != 0) {
			// 	it++;
			// 	if (it->compare(";") != 0) {
			// 		std::string tmp = *it;
			// 		sv.error_page.insert(std::make_pair(atoi(tmp.c_str()), value));
			// 	}
			// }
			it++;
			if (it == ite || it->compare(";") == 0)
				throw std::logic_error("argument not find");
			std::string tmp = *it;
			it++;
			if (it != ite || it->compare(";") != 0)
				sv.error_page.insert(std::make_pair(tmp, *it));
		}
		if (it->compare("client_max_body_size") == 0) {
			it++;
			if (it == ite || it->compare(";") == 0)
				throw std::logic_error("argument not find");
			sv.client_max_body_size = *it;
		}
		if (it->compare("root") == 0) {
			it++;
			if (it == ite || it->compare(";") == 0)
				throw std::logic_error("argument not find");
			sv.root = *it;
		}
		if (it->compare("index") == 0) {
			while (it != ite && it->compare(";") != 0) {
				it++;
				if (it != ite && it->compare(";") != 0){
					sv.index.push_back(*it);
				}
			}
		}
		if (it->compare("autoindex") == 0) {
			it++;
			if (it == ite || it->compare(";") == 0)
				throw std::logic_error("argument not find");
			sv.autoindex = *it;
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
				throw std::logic_error("bracket is missing");
			else {
				it--;
				while (it != ite && it->compare("}") != 0) {
					if (it->compare("location") == 0) {
						int open = 1;
						// while (it != ite && it->compare("}") != 0) {
						std::cout << "new LOC" << std::endl;
						while (it != ite && open > 0) {
							// std::cout << "it : " << *it << " open : " << open << std::endl;
							loc.push_back(*it);
							vec.erase(it);
							if (it != ite && it->compare("}") == 0)
								open--;
							else if (it != ite && it->compare("location") == 0)
								open++;
						}
						loc.push_back(*it);
						vec.erase(it);
						ite = vec.end();
						continue ;
					}
					serv.push_back(*it);
					vec.erase(it);
				}
				serv.push_back(*it);
				vec.erase(it);
				// std::cout << "print LOC" << std::endl;
				// for (std::vector<std::string>::iterator it = loc.begin(); it != loc.end(); it++) {
				// 	std::cout << "it : " << *it << std::endl;
				// }
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
	std::vector<s_server_config>::iterator it = _serverConf.begin();
	std::vector<s_server_config>::iterator ite = _serverConf.end();
	for (; it != ite; it++) {
		std::cout << "[server]" << std::endl;
		if (!it->listen.empty()) {
			std::cout << "listen : " << it->listen << std::endl;
		}
		if (!it->server_name.empty()) {
			for (std::list<std::string>::iterator i = it->server_name.begin(); i != it->server_name.end(); i++)
				std::cout << "server_name : " << *i << std::endl;
		}
		if (!it->error_page.empty()) {
			for (std::map<std::string, std::string>::iterator i = it->error_page.begin(); i != it->error_page.end(); i++)
				std::cout << "error_page key : " << i->first << " value : " << i->second << std::endl;
		}
		if (!it->client_max_body_size.empty()) {
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
				if (!i->path.empty()) {
					std::cout << "path : " << i->path << std::endl;
				}
				if (!i->root.empty()) {
					std::cout << "root : " << i->root << std::endl;
				}
				if (!i->index.empty()) {
					for (std::list<std::string>::iterator j = i->index.begin(); j != i->index.end(); j++)
						std::cout << "index : " << *j << std::endl;
				}
				if (!i->autoindex.empty())
					std::cout << "autoindex : " << i->autoindex << std::endl;
				if (!i->fastcgi_pass.empty())
					std::cout << "fastcgi_pass : " << i->fastcgi_pass << std::endl;
				// if (!i->location.empty())
				// 	std::cout << "location : " << i->location.begin()->path << std::endl;
				for (std::vector<s_location>::iterator k = i->location.begin(); k != i->location.end(); k++) {
					std::cout << "[location] of location" << std::endl;
					if (!k->path.empty()) {
						std::cout << "path : " << k->path << std::endl;
					}
					if (!k->root.empty()) {
						std::cout << "root : " << k->root << std::endl;
					}
					if (!k->index.empty()) {
						for (std::list<std::string>::iterator j = k->index.begin(); j != k->index.end(); j++)
							std::cout << "index : " << *j << std::endl;
					}
					if (!k->autoindex.empty())
						std::cout << "autoindex : " << k->autoindex << std::endl;
					if (!k->fastcgi_pass.empty())
						std::cout << "fastcgi_pass : " << k->fastcgi_pass << std::endl;
					for (std::vector<s_location>::iterator k2 = k->location.begin(); k2 != k->location.end(); k2++) {
						std::cout << "[location] of location bis" << std::endl;
						if (!k2->path.empty()) {
							std::cout << "path : " << k2->path << std::endl;
						}
						if (!k2->root.empty()) {
							std::cout << "root : " << k2->root << std::endl;
						}
						if (!k2->index.empty()) {
							for (std::list<std::string>::iterator j = k2->index.begin(); j != k2->index.end(); j++)
								std::cout << "index : " << *j << std::endl;
						}
						if (!k2->autoindex.empty())
							std::cout << "autoindex : " << k2->autoindex << std::endl;
						if (!k2->fastcgi_pass.empty())
							std::cout << "fastcgi_pass : " << k2->fastcgi_pass << std::endl;
					}
				}
			}
		}
	}
}

bool		Cluster::initCluster(){
	this->_config = _getListen();
	this->t_conf::iterator it = this->_config.begin();
	this->t_conf::iterator ite = _this->config.end();
	for ( ; it != ite; it++) {

		// for each port we instatiate server
		int port = it->first;
		this->_servers.insert(std::make_pair(port, new Server(port, it->second)));
		long		fd = 0;
		if (this->_servers[port]->setup() != -1)
		{
			fd = this->_servers[port]->getSocket();
			FD_SET(fd, &(this->_msfd)); // bind socket (fd) to master fd (_msfd) see select doc.
			if (fd > _max_sk)
				this->_max_sk = fd;
		}
		else {
			this->_servers.erase(port); // if failed setup, remove from server list
		}
	}
	return true;
}

bool		Cluster::launch(){

	struct timeval tv; // timevalue is only used for select's last parameter: timeout value.
	int recVal = 0;
	tv.tv_sec = 1;
	while(true)
	{
		// We're making a copy because select() modifies the master fd set(_msfd).
		fd_set rfds;
		FD_ZERO(&rfds);
		memcpy(&rfds, &(this->_msfd), sizeof(this->_msfd));

		// std::flush outputs cout to the console.
		std::cout << "\rWaiting for client" << std::flush; 

		recVal = select(_max_sk + 1, &rfds, NULL, NULL, &tv); // wait for change on rfds
		switch(recVal)
		{
			case(0):{//Timeout
				break;
			}
			case(-1):
			{
				perror("\nSelect failed");
				for (std::map<int, Server *>::iterator it = _servers.begin(); it != _servers.end(); it++)
					close(it->second->getSocket());
				return false;
			}
			default: //change happened
			{
				// select() changes the fd_set : it now contains ONLY the ones ready for reading.
				for (std::map<int, Server *>::iterator it = _clients.begin(); it != _clients.end(); it++){ //iterate on connected clients
					if (FD_ISSET(it->first, &rfds)){ // search the client who send us smth
						int client_fd = it->first;
						int ret = it->second->listenClient(client_fd);
						if ( ret <= 0){ // listen to it
							std::cout << RED << "\nConnection "<< client_fd<< " closed." << WHITE << std::endl;
							close(it->first);
							FD_CLR(it->first, &rfds);
							FD_CLR(it->first, &(this->_msfd));
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
							FD_SET(connection, &(this->_msfd)); // root client socket (connection) to master fd
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

Cluster::Cluster(){
	_max_sk = -1;
}

Cluster::t_conf Cluster::_getListen()
{
	/*
		get all port that we need to listen to
		create map of port & vector of server
	*/
	
	t_conf res;
	std::vector<s_server_config>::iterator it = _serverConf.begin();
	std::vector<s_server_config>::iterator ite = _serverConf.end();
	for ( ; it != ite; it++) {
		int port;
		int ret;
		std::string tmp = it->listen;
		if ((ret = tmp.find(':')))
			port = atoi(tmp.substr(ret + 1, tmp.length() - ret).c_str());
		else
			port = atoi(tmp.c_str());
		res[port].push_back(*it);
	}
	// display map
	/*t_conf::iterator b;
	for (b = res.begin(); b != res.end(); b++)
	{
		std::cout << b->first << std::endl;
		std::vector<s_server_config>::iterator z;
		for (z = b->second.begin(); z != b->second.end(); z++)
			std::cout << z->server_name.front() << std::endl;
	}*/
	return res;
}