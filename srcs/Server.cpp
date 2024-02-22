/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbengoec <pbengoec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/12 10:10:40 by phijano-          #+#    #+#             */
/*   Updated: 2024/02/22 20:05:04 by pbengoec         ###   ########.fr       */
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
	serverAddress->sin_family = AF_INET;
	serverAddress->sin_port = htons(8080);
	serverAddress->sin_addr.s_addr = inet_addr("0.0.0.0");
	serverSocket = socket(AF_INET, SOCK_STREAM, 0);
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

void	Server::initServer()
{
	char buffer[30720];
	long bytes;
	std::vector<pollfd> fds(5);

	if (bind(serverSocket, (struct sockaddr*) serverAddress, addressLen) < 0)
	{
		std::cout<<config->getPort()<< std::endl;
		std::cout << "Error connect socket to adress" << std::endl;
		close(serverSocket);
		exit(1);
	}
		
	if (listen(serverSocket, 20) < 0 )
		std::cout << "Error listen" << std::endl;

	if (fcntl(serverSocket, F_SETFL, O_NONBLOCK) < 0) 
       std::cout << "Error listen" << std::endl;
	
	std::cout << "listening: address " << inet_ntoa(serverAddress->sin_addr) << " port " << ntohs(serverAddress->sin_port) << std::endl;

	fds[0].fd = serverSocket;
	fds[0].events = POLLIN;
	while (true)
	{
		//Waiting for events
		int activity = poll(fds.data(), fds.size(), -1);
		if (activity < 0)
			std::cout<<"adios"<<std::endl;
		//New client socket
		if (fds[0].revents & POLLIN)
		{
			acceptSocket = accept(serverSocket, (sockaddr *) &serverAddress, &addressLen);
			if (acceptSocket < 0)
				std::cout << "Error accepting" << std::endl;
			//Assigning the new Client socket to the pollfd list that is empty
			for (size_t i = 1; i < fds.size(); i++)
			{
				if (fds[i].fd == 0)
				{
					fds[i].fd = acceptSocket;
					fds[i].events = POLLIN;
					break;
				}
			}	
		}
		for (size_t i = 1; i< fds.size(); i++)
		{
			if (fds[i].revents & POLLIN) {
				bytes = read(fds[i].fd, buffer, 30720);
				// std::cout << buffer << std::endl;
				if (bytes < 0)
					std::cout << "Error reading " << strerror(errno) << std::endl;
				//Client disconnected
				if (bytes == 0)
				{
					std::cout << "Host disconnected" <<std::endl;
					close(fds[i].fd);
					fds[i].fd = 0;	
				}
				else
				{
					Request request(buffer);
					Response response(request);
					long bytesSent;
					bytesSent = write(acceptSocket, response.getResponse().c_str(), response.getResponse().size());
					if (bytesSent < 0)
						std::cout << "Error writing" << std::endl;
				}
			}
		}
	}
	close(serverSocket);
}