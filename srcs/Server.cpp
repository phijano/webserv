/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbengoec <pbengoec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/12 10:10:40 by phijano-          #+#    #+#             */
/*   Updated: 2024/02/27 19:11:43 by pbengoec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	char buffer[30720];
	long bytes;
	std::vector<pollfd> fds(4);

	connectServerAddress();
	fds[0].fd = serverSocket;
	fds[0].events = POLLIN;
	std::cout << "listening: address " << inet_ntoa(serverAddress.sin_addr) << " port " << ntohs(serverAddress.sin_port) << std::endl;
	while (true)
	{
		int activity = poll(fds.data(), fds.size(), -1);
		if (activity < 0)
			std::cout<<"Error activity"<<std::endl;
		if (fds[0].revents & POLLIN)
		{
			for (size_t i = 1; i < fds.size(); i++)
			{
				if (fds[i].fd == 0)
				{
					fds[i].fd = accept(serverSocket, (sockaddr *) &serverAddress, &addressLen);
					if (fds[i].fd < 0)
						std::cout << "Error accepting" << std::endl;
					fds[i].events = POLLIN;
					break;
				}
			}	
		}
		for (size_t i = 1; i< fds.size(); i++)
		{
			if (fds[i].revents & POLLIN) {
				
				bytes = read(fds[i].fd, buffer, 30720);
				if (bytes < 0)
					std::cout << "Error reading " << strerror(errno) << std::endl;
				//Client disconnected
				if (bytes == 0)
				{
					std::cout << "Host disconnected\n" <<std::endl;
					close(fds[i].fd);
					fds[i].fd = 0;
				}
				else
					fds[i].events = POLLOUT;
			}
			if (fds[i].revents & POLLOUT) 
			{	
				// Request request(buffer);
				// Response response(request, config[0]);
				std::string body = "<h1>Hello, world!</h1>";
				std::string response = "HTTP/1.1 200 OK\r\nContent-Length: " + std::to_string(body.length()) + "\r\n\r\n" + body;
				write(fds[i].fd, response.c_str(), response.length());
				fds[i].events = POLLIN;
			}
		}
		close(serverSocket);
	}
}