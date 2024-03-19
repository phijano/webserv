#include "ServerManager.hpp"

ServerManager::ServerManager()
{}

ServerManager::ServerManager(std::vector<Config> configs)
{
	for (size_t i = 0; i < configs.size(); i++)
	{
		addServer(&configs[i]);
	}
	run();
}

ServerManager::~ServerManager(){}

void	ServerManager::addServer(Config *config)
{
	Server	server(config);
	struct pollfd connection;

	connection.fd = server.getServerSocket();
	connection.events = POLLIN;
	this->conn.push_back(connection);
	this->servers.push_back(server);
}

void	ServerManager::newClient(Server server)
{
	struct pollfd connection;
	int client;

	client = accept(server.getServerSocket(), (struct sockaddr *)&server.getServerAddress(), &server.getServerAddressLen());
	if (fcntl(client, F_SETFL, O_NONBLOCK) < 0)
		std::cout<<"Error"<<std::endl;
	connection.fd = client;
	connection.events = POLLIN;
	this->conn.push_back(connection);
	this->client.push_back(client);
}

void	ServerManager::serverEvent()
{
	for (size_t i = 0; i < servers.size(); i++)
	{
		if (this->conn[i].revents & POLLIN)
		{
			newClient(servers[i]);
		}
	}
}

void	ServerManager::run()
{
	int	activity;
	int initialSize;

	while (1)
	{
		initialSize = this->conn.size();
		activity = poll(this->conn.data(), this->conn.size(), -1);
		serverEvent();
		clientEvent();
	}
}