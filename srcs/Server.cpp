#include "Server.hpp"

Server::Server()
{
	std::cout << "Server default constructor called" << std::endl;
	serverSocket = socket(PF_INET, SOCK_STREAM, 0);
	if (serverSocket < 0)
	{
		//handle error
		std::cout << "Error socket" << std::endl;
	}
}

Server::Server(Config *config): config(config)
{
	setServerAddress(this->config);
	serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	//transform this error in try catch
	if (serverSocket < 0)
		std::cout << "Error socket" << std::endl;
	initServer();
}

Server::Server(const Server& other)
{
	*this = other;
}

Server::~Server()
{
	close(serverSocket);
}

void	Server::setServerAddress(Config *config)
{
	this->serverAddress.sin_family = AF_INET;
	this->serverAddress.sin_port = htons(config->getPort());
	this->serverAddress.sin_addr.s_addr = inet_addr(config->getHost().c_str());
	this->addressLen = sizeof(serverAddress);
}

void	Server::connectServerAddress(void)
{
	int reuse = 1;

	if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) == -1) {
        std::cerr << "Error establishing SO_REUSEADDR." << std::endl;
        close(serverSocket);
		exit (1);
    }

	if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEPORT, &reuse, sizeof(reuse)) == -1) {
        std::cerr << "Error establishing SO_REUSEADDR." << std::endl;
        close(serverSocket);
		exit (1);
    }

	if (bind(serverSocket, (struct sockaddr *) &serverAddress, addressLen) < 0)
	{
		std::cout << "Error connect socket to adress" << std::endl;
		close(serverSocket);
		exit(1);
	}

	if (listen(serverSocket, 20) < 0 )
		std::cout << "Error listen" << std::endl;

	if (fcntl(serverSocket, F_SETFL, O_NONBLOCK) < 0) 
       std::cout << "Error fcntl" << std::endl;
}

void	Server::initServer()
{
	char buffer[1024] = {0};
	int acceptSocket;
	std::vector<pollfd> fds(10);
	int activity;

	connectServerAddress();
	std::cout << "listening: address " << inet_ntoa(serverAddress.sin_addr) << " port " << ntohs(serverAddress.sin_port) << std::endl;
	fds[0].fd = serverSocket;
	fds[0].events = POLLIN;
	while (1)
	{
		activity = poll(fds.data(), fds.size(), -1); // unused variable
		if (fds[0].revents & POLLIN) // if (incoming connection request from a client)
		{
			acceptSocket = accept(serverSocket, (struct sockaddr *)&serverAddress, &addressLen);
			if (fcntl(acceptSocket, F_SETFL, O_NONBLOCK) < 0)
				std::cout<<"Error"<<std::endl;
			for (size_t i = 1; i < fds.size(); i++)
			{
				if (fds[i].fd == 0)
				{
					//std::cout<<"New Client connected to socket number "<< i <<std::endl;
					fds[i].fd = acceptSocket;
					fds[i].events = POLLIN;
					break;
				}
			}
		}
		else
		{
			for (size_t i = 1; i < fds.size(); i++)
			{
				if (fds[i].revents & POLLIN && fds[i].fd != 0)
				{
					ssize_t bytesRead = recv(fds[i].fd, buffer, sizeof(buffer), 0);
					if (bytesRead >= 0) 
					{
						if (bytesRead > 0)
						{
							buffer[bytesRead] = '\0';
							fds[i].events = POLLOUT;
						}
					}
					else
						std::cerr << "Error de lectura del cliente\n";
					
				}
				if (fds[i].revents & POLLOUT)
				{
					//std::string body = "<h1>Hello, world!</h1>";
					//std::string response = "HTTP/1.1 200 OK\r\nConnection: Keep-Alive\r\nContent-Length: " + std::to_string(body.length()) + "\r\nKeep-Alive: timeout=10, max=100\r\n\r\n" + body;
					//send(fds[i].fd, response.c_str(), response.length(), 0);
					//std::cout << "buffer: " << buffer << std::endl;
					Request request(buffer);
					std::cout << "Buffer: " << buffer << std::endl << "END buffer" << std::endl;
					std::cout << "Request: " << request << std::endl << "END Request" << std::endl;
					Response response(request, config[0]);
					//std::cout<< response.getResponse().c_str()<<std::endl;
					send(fds[i].fd, response.getResponse().c_str(), response.getResponse().size(), 0);
					fds[i].events = POLLIN;
				}
				if (fds[i].revents & POLLHUP)
				{
					std::cout << "Client associated to socket number "<<i<<" has disconnected\n";
					close(fds[i].fd);
					fds[i].fd = 0;
				}
				if (fds[i].revents & POLLERR)
				{
					std::cout << "There has been an error[POLLERR]\n";
					close(fds[i].fd);
					fds[i].fd = 0;
				}
			}
		}
	}
	close(serverSocket);
}