#include <iostream>
#include <iterator>
#include <string.h>

#include "config_context.hpp"

namespace cf {

	std::vector<s_location>		configContext::_setupLocation(std::vector<std::string> &loc) {
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

	s_server	configContext::_setupServer(std::vector<std::string> &serv, std::vector<std::string> &loc) {
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
					if (it->compare(";") != 0)
						sv.error_page.insert(std::make_pair(stoi(*it), value));
				}
			}
			if (it->compare("client_max_body_size") == 0) {
				it++;
				sv.client_max_body_size = stoi(*it);
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

	void	configContext::exploitTokens(std::vector<std::string> &tokens) {
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
					_server.push_back(_setupServer(serv, loc));
					serv.clear();
					loc.clear();
					ite = vec.end();
					continue ;
				}
			}
			it++;
		}
	}

	void	configContext::printConfig(void) {
		std::vector<s_server>::iterator it = _server.begin();
		std::vector<s_server>::iterator ite = _server.end();
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

}
