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
	sockaddr_in clientAddress;
    socklen_t clientAddressLen = sizeof(clientAddress);
	Client client;
	int acceptClient;

	acceptClient = accept(server.getServerSocket(), (struct sockaddr*)&clientAddress, &clientAddressLen);
	if (fcntl(acceptClient, F_SETFL, O_NONBLOCK) < 0)
		std::cout<<"Error"<<std::endl;
	connection.fd = acceptClient;
	connection.events = POLLIN;
	client = Client(server.getConfig(), acceptClient);
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
	char buffer[1024];
	Request request;
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
					request = Request(buffer);
					conn[i].events = POLLOUT;
				}
			}
			else
				std::cerr << "Error de lectura del cliente\n";
		}
		if (conn[i].revents & POLLOUT)
		{
			response = Response(request, clients[i - servers.size()].getConfig());
			send(conn[i].fd, response.getResponse().c_str(), response.getResponse().size(), 0);
			conn[i].events = POLLIN;
		}
		if (conn[i].revents & POLLHUP)
		{
			std::cout << "Client associated to socket number "<<conn[i].fd<<" is disconnected\n";
			removeClient(i);
		}
		if (conn[i].revents & POLLERR)
		{
			std::cout << "There have benn an error[POLLERR]\n";
			removeClient(i);
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