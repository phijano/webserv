#include "ServerManager.hpp"

ServerManager::ServerManager()
{}

ServerManager::ServerManager(std::vector<Config> configs)
{
	int			dup;
	std::string	host;
	int			port;

	for (size_t i = 0; i < configs.size(); i++)
	{	
		host = configs[i].getHost();
		port = configs[i].getPort();
		dup = 0;
		for (size_t j = 0; j < i; j++)
			if (host == configs[j].getHost() and port == configs[j].getPort())
				dup = 1;
		if (!dup)
			addServer(&configs[i]);
	}
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
	sockaddr_in clientAddress;
    socklen_t clientAddressLen = sizeof(clientAddress);
	Client client;
	int acceptClient;

	acceptClient = accept(server.getServerSocket(), (struct sockaddr*)&clientAddress, &clientAddressLen);
	if (fcntl(acceptClient, F_SETFL, O_NONBLOCK) < 0)
		std::cout<<"Error"<<std::endl;
	connection.fd = acceptClient;
	connection.events = POLLIN;
	client = Client(server.getConfig(), acceptClient, clientAddress.sin_addr.s_addr);
	std::cout<<"Cliente conectado en socket "<<client.getId()<<std::endl;
	this->conn.push_back(connection);
	this->clients.push_back(client);
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

void	ServerManager::removeClient(int position)
{
	close(conn[position].fd);
	conn.erase(conn.begin() + position);
	clients.erase(clients.begin() + (position - servers.size()));
}

void	ServerManager::clientEvent()
{
	char buffer[2048];
	Response response;
	ssize_t bytesRead;
	for (size_t i = servers.size(); i < conn.size(); i++)
	{
		if (conn[i].revents & POLLIN)
		{
			bytesRead = recv(conn[i].fd, buffer, sizeof(buffer), 0);
			if (bytesRead >= 0) 
			{
				if (bytesRead > 0)
				{
					buffer[bytesRead] = '\0';
					clients[i - servers.size()].setRequest(Request(buffer));
					conn[i].events = POLLOUT;
				}
			}
			else
			{
				std::cerr << "Error de lectura del cliente\n";
				removeClient(i);
			}
		}
		if (conn[i].revents & POLLOUT)
		{
			response = Response(clients[i - servers.size()].getRequest(), clients[i - servers.size()].getConfig());
			send(conn[i].fd, response.getResponse().c_str(), response.getResponse().size(), 0);
			conn[i].events = POLLIN;
		}
		if (conn[i].revents & POLLHUP || conn[i].revents & POLLERR)
		{
			std::cout << "Client associated to socket number "<<conn[i].fd<<" is disconnected\n";
			removeClient(i);
		}
	}
}

void	ServerManager::run()
{
	int	activity;

	while (1)
	{
		activity = poll(this->conn.data(), this->conn.size(), -1);
		serverEvent();
		clientEvent();
	}
}
