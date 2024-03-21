#ifndef SERVERMANAGER_HPP
# define SERVERMANAGER_HPP

# include "Library.hpp"
# include "Server.hpp"
# include "Client.hpp"
# include "Response.hpp"
# include "Request.hpp"
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
		void	serverEvent();
		void	clientEvent(size_t initialSize);
		void	removeClient(int fd);
		void	newClient(Server server);
		void	run();

};

#endif
