#ifndef SERVERMANAGER_HPP
# define SERVERMANAGER_HPP

# include "Library.hpp"
class Server;
class Client;
class Config;
class ServerManager
{
	private:
		std::vector <Server> servers;
		std::vector <Client> clients;
		std::vector <pollfd> conn;
		std::vector <Config> configs;
	public:
		ServerManager();
		ServerManager(std::vector<Config> configs);
		~ServerManager();

};

#endif
