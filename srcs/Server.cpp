/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbengoec <pbengoec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/12 10:10:40 by phijano-          #+#    #+#             */
/*   Updated: 2024/03/05 14:28:02 by pbengoec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server()
{
	serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	//transform this error in try catch
	if (serverSocket < 0)
		std::cout << "Error socket" << std::endl;
	std::cout<<"Server initialized with socket num "<<serverSocket<<std::endl;
}

Server::Server(Config *config): config(config)
{
	setServerAddress(this->config);
	connectServerAddress();
	std::cout<<"Server initialized with socket num "<<serverSocket<<std::endl;
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
        std::cerr << "Error establishing SO_REUSEADDR. in server num "<< serverSocket << std::endl;
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

void	Server::initServer()
{
	char buffer[1024] = {0};
	int acceptSocket;
	std::vector<pollfd> fds(3);
	int activity;

	fds[0].fd = serverSocket;
	fds[0].events = POLLIN;
	while (1)
	{
		activity = poll(fds.data(), fds.size(), -1);
		if (fds[0].revents & POLLIN)
		{
			acceptSocket = accept(serverSocket, (struct sockaddr *)&serverAddress, &addressLen);
			if (fcntl(acceptSocket, F_SETFL, O_NONBLOCK) < 0)
				std::cout<<"Error"<<std::endl;
			for (size_t i = 1; i < fds.size(); i++)
			{
				if (fds[i].fd == 0)
				{
					std::cout<<"New Client connected to socket number "<< i <<std::endl;
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
					std::cout<<"hola"<<std::endl;
					if (bytesRead >= 0) 
					{
						if (bytesRead > 0)
							fds[i].events = POLLOUT;		
					}
					else
						std::cerr << "Error de lectura del cliente\n";
					
				}
				if (fds[i].revents & POLLOUT)
				{
					// std::string body = "<h1>Hello, world!</h1>";
					// std::string response = "HTTP/1.1 200 OK\r\nConnection: Keep-Alive\r\nContent-Length: " + std::to_string(body.length()) + "\r\nKeep-Alive: timeout=10, max=100\r\n\r\n" + body;
					// send(fds[i].fd, response.c_str(), response.length(), 0);
					Request request(buffer);
					Response response(request, config[0]);
					send(fds[i].fd, response.getResponse().c_str(), response.getResponse().size(), 0);
					fds[i].events = POLLIN;
				}
				if (fds[i].revents & POLLHUP)
				{
					std::cout << "Client associated to socket number "<<i<<" is disconnected\n";
					close(fds[i].fd);
					fds[i].fd = 0;
				}
				if (fds[i].revents & POLLERR)
				{
					std::cout << "There have benn an error[POLLERR]\n";
					close(fds[i].fd);
					fds[i].fd = 0;
				}
			}
		}
		
	}
	close(serverSocket);
}