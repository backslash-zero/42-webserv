#pragma once

class Config
{
public:
	Server(void);
	~Server(void);

private:
	// Server configuration settings
	int port;
	int host;
	std::string server_name;
}