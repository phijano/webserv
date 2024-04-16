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
			if (port == configs[j].getPort())
				dup = 1;
		if (!dup)
			addServer(configs[i]);
	}
	initialSize = 0;
}

ServerManager::~ServerManager(){}

void	ServerManager::addServer(Config config)
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

	client.setTime(clock() - initialTime);
	acceptClient = accept(server.getServerSocket(), (struct sockaddr*)&clientAddress, &clientAddressLen);
	std::cout<<"Aceptando a = "<<acceptClient<<std::endl;
	if (acceptClient == -1) {
        std::cerr << "Error accepting new client: " << strerror(errno) << std::endl;
        return;
    }
	if (fcntl(acceptClient, F_SETFL, O_NONBLOCK) < 0)
	{
		std::cout<<"Error fcntl client"<<std::endl;
		exit(1);
	}
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
			std::cout<<"Server conectado en el socket "<< this->conn[i].fd<<std::endl;
			newClient(servers[i]);
		}
	}
}

void	ServerManager::removeClient(int position)
{
	clients[position - servers.size()].remove();
	conn.erase(conn.begin() + position);
	clients.erase(clients.begin() + (position - servers.size()));
}

void	ServerManager::clientEvent()
{
	char buffer[2048];
	Response response;
	ssize_t bytesRead;

	for (size_t i = initialSize; i >= servers.size(); i--)
	{
		if (conn[i].revents & POLLIN)
		{
			bytesRead = recv(conn[i].fd, buffer, sizeof(buffer), 0);
			if (bytesRead >= 0) 
			{
				if (bytesRead > 0)
				{
					buffer[bytesRead] = '\0';
					std::cout<<buffer<<std::endl;
					clients[i - servers.size()].setRequest(Request(buffer));
					conn[i].events = POLLOUT;
				}
			}
			else
			{
				std::cerr << "Error reading client num "<<conn[i].fd<<std::endl;
				removeClient(i);
				continue;
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

void	ServerManager::checkTimeOut()
{
	for (size_t i = initialSize; i >= servers.size(); i--)
	{
		if (isTimeOut(clients[i - servers.size()]))
		{
			// std::cout<<"Client Time "<<clients[i].getTime()<<std::endl;
			// std::cout<<"Initial Time "<<initialTime<<std::endl;
			std::cout<<"Cliente desconectado desde timeout"<<std::endl;
			removeClient(i);
		}
	}
}

bool	ServerManager::isTimeOut(Client client)
{
	if (static_cast<double>(client.getTime()/CLOCKS_PER_SEC) > 0.1)
		return true;
	return false;
}

void sleep_ms(uint32_t milliseconds) {
    timespec ts;
    ts.tv_sec = milliseconds / 1000;
    ts.tv_nsec = (milliseconds % 1000) * 1000000;
    nanosleep(&ts, nullptr);
}

void	ServerManager::run()
{
	int	activity;

	initialTime = clock();
	while (1)
	{
		initialSize = this->conn.size() - 1;
		activity = poll(this->conn.data(), this->conn.size(), 5000);
		std::cout<<"------NEW LAP--------"<<std::endl;
		std::cout<<"Activity = "<<activity<<std::endl;
		if (activity == -1)
		{
			std::cout<<"Error with poll"<<std::endl;
			exit(1);
		}
		else if (activity == 0)
		{
			continue;
			checkTimeOut();
		}
		else
		{
			serverEvent();
			clientEvent();
		}
		sleep_ms(5);
	}
}
