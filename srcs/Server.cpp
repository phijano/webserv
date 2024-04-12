#include "Server.hpp"

Server::Server()
{
	serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocket < 0)
		std::cout << "Error socket" << std::endl;
	std::cout<<"Server initialized with socket num "<<serverSocket<<std::endl;
}

Server::Server(Config config): config(config)
{
	serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocket < 0)
		std::cout << "Error socket" << std::endl;
	std::cout<<"Server initialized with socket num "<<serverSocket<<std::endl;
	setServerAddress();
	connectServerAddress();
}

Server::Server(const Server& other):config(Config(other.config))
{
	*this = other;
}

Server	&Server::operator=(const Server &copy)
{
	if (this != &copy)
	{
		serverAddress = copy.serverAddress;
		serverSocket = copy.serverSocket;
	}
	return *this;
}

Server::~Server()
{}

void	Server::setServerAddress()
{
	this->serverAddress.sin_family = AF_INET;
	this->serverAddress.sin_port = htons(config.getPort());
	this->serverAddress.sin_addr.s_addr = inet_addr(config.getHost().c_str());
	this->addressLen = sizeof(serverAddress);
}

void	Server::connectServerAddress(void)
{
	int reuse = 1;

	if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) == -1) {
        std::cerr << "Error establishing SO_REUSEADDR. Server num "<< serverSocket << std::endl;
        close(serverSocket);
		exit (1);
    }

	if (bind(serverSocket, (struct sockaddr *) &serverAddress, addressLen) < 0)
	{
		std::cout << "Error connect socket to adress" << std::endl;
		close(serverSocket);
		exit(1);
	}

	if (listen(serverSocket, 512) < 0 )
		std::cout << "Error listen" << std::endl;
		
	if (fcntl(serverSocket, F_SETFL, O_NONBLOCK) < 0) 
       std::cout << "Error fcntl" << std::endl;

	std::cout << "listening: address " << inet_ntoa(serverAddress.sin_addr) << " port " << ntohs(serverAddress.sin_port) << std::endl;

}

int	Server::getServerSocket(void)
{
	return (this->serverSocket);
}

sockaddr_in	Server::getServerAddress(void)
{
	return (this->serverAddress);
}

socklen_t	Server::getServerAddressLen(void)
{
	return (this->addressLen);
}

Config		Server::getConfig(void)
{
	return (this->config);
}

std::ostream& operator<<(std::ostream& os, Server& server)
{
	 os << "Host: " << server.getServerAddress().sin_addr.s_addr << std::endl;
	 os << "Port: " << server.getServerAddress().sin_port << std::endl;
	 return os;
}