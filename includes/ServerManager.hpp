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
	public:
		ServerManager();
		ServerManager(std::vector<Config> configs);
		~ServerManager();

		void	addServer(Config *config);
		void	serverEvent(Server config);
		void	newClient();
		void	run();

};

#endif
