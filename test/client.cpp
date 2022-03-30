
// Client side C/C++ program to demonstrate Socket programming
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

#include <iostream>
#include <fstream>
#include <string>

#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define RESET "\033[0m"

int		choose_port(void)
{
	std::string		port;

	std::cout << GREEN << "Enter PORT :" << std::endl;
	getline(std::cin, port);

	return (atoi(port.c_str()));
}

int		choose_type(void)
{
	std::string		choice;

	std::cout << GREEN << "Specific test or automatic ?" << std::endl;
	std::cout << GREEN << "(a) Specific" << std::endl;
	std::cout << GREEN << "(b) Automatic" << std::endl;
	getline(std::cin, choice);
	if (choice == "a")
		return 0;
	else if (choice == "b")
		return 1;
	else {
		std::cout << RED << "Invalid input." << std::endl;
		return choose_type();
	}
}

std::string		choose_category(void)
{
	std::string		choice;

	std::cout << GREEN << "Choose category :" << std::endl;
	std::cout << GREEN << "(a) header" << std::endl;
	getline(std::cin, choice);
	if (choice == "a")
		return "header";
	else {
		std::cout << RED << "Invalid input." << std::endl;
		return choose_category();
	}
}


std::string		choose_idx(void)
{
	std::string		choice;

	std::cout << GREEN << "Choose file idx (0-5)" << std::endl;
	getline(std::cin, choice);
	return (choice);
}

std::string		choose_responseFolder(void)
{
	std::string		choice;

	std::cout << GREEN << "Choose response folder (exemple : response)" << std::endl;
	getline(std::cin, choice);
	return (choice);
}

std::string		sendRequest(int port, std::string filename, std::string responseFolder)
{
	int					sock;
	struct sockaddr_in	serv_addr;
	char				buffer[4096] = {0};
	std::fstream		file;
	std::string			content;
	std::cout << filename << std::endl;
	std::ofstream		myfile;
  	myfile.open(responseFolder, std::ios::app);

	file.open(filename.c_str());
	content.assign(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
	file.close();

	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		std::cout << std::endl << RED << "< Socket creation error >" << RESET << std::endl << std::endl;
		return "NULL";
	}

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(port);


	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	{
		std::cout << std::endl << RED << "< Connection failed >" << RESET << std::endl << std::endl;
		return "NULL";
	}

	content += "\r\n\r\n";

	std::cout << std::endl << "Sending :" << std::endl;
	std::cout << "[" << RED << content << RESET << "]" << std::endl << std::endl;

	myfile << "// Sended //" <<std::endl;
	myfile << content << std::endl;
	
	send(sock, content.c_str(), content.size(), 0);
	read(sock, buffer, 4095);

	std::cout << std::endl << "Response :" << std::endl;
	std::cout << "[" << YELLOW << std::string(buffer) << RESET << "]" << std::endl << std::endl;

	myfile << "\n// Received //\n";
	myfile << std::string(buffer) << std::endl;
	myfile << "\t\t//////////" << std::endl;

  	myfile.close();
	close(sock);

	return std::string(buffer);
}

void	automatic(int port, std::string responseFodler){
	std::string folder[1] = { "header"};
	for (int i = 0; i < 1; i++){
		std::fstream		file;
		std::string			response;
		std::string current = folder[i];
		int nbFile;
		file.open("test/exemple/" + current + "/response");
		response.assign(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
		file.close();
		nbFile = atoi(response.substr(0, response.find_first_of('\n')).c_str());

		for (int n = 0; n < nbFile; n++){
			std::string ret = sendRequest(port, "test/exemple/" + current + "/" + std::to_string(n), responseFodler);
			if (ret == "NULL")
				break ;
		}
	}
}

int		main(void)
{
	int				port;
	int				type;
	std::string 	responseFodler;
	std::string		filename;
	std::ofstream		myfile;

	port = choose_port();
	responseFodler = "test/response/" + choose_responseFolder() + ".txt";
	type = choose_type();
  	myfile.open(responseFodler, std::ios::trunc);
  	myfile.close();

	while (true)
	{
		if (type == 0) {
			std::string category = choose_category();
			std::string idx = choose_idx();
			std::string ret = sendRequest(port, "test/exemple/" + category + "/" + idx, responseFodler);
			if (ret == "NULL")
				break ;
		}
		else if (type == 1){
			automatic(port, responseFodler);
			break;
		}
	}
	return (0);
}