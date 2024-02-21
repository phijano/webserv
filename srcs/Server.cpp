/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbengoec <pbengoec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/12 10:10:40 by phijano-          #+#    #+#             */
/*   Updated: 2024/02/21 21:05:48 by pbengoec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server()
{
	std::cout << "Server default constructor called" << std::endl;
	_socket = socket(PF_INET, SOCK_STREAM, 0);
	if (_socket < 0)
	{
		//handle error
		std::cout << "Error socket" << std::endl;
	}
}

Server::Server(std::string ip, int port): _ip(ip), _port(port) , _addressLen(sizeof(_socketAddress))
{
	_socketAddress.sin_family = AF_INET;
	_socketAddress.sin_port = htons(_port);
	_socketAddress.sin_addr.s_addr = inet_addr(_ip.c_str());
	_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (_socket < 0)
		std::cout << "Error socket" << std::endl;
	initServer();
}

Server::Server(const Server& other)
{
	*this = other;
}

Server& Server::operator=(const Server& other)
{
	_ip = other._ip;
	_port = other._port;
	_socket = other._socket;
	return *this;
}

Server::~Server()
{
	close(_socket);
}

void	Server::initServer()
{
	char buffer[30720];
	long bytes;
	std::vector<pollfd> fds(5);

	if (bind(_socket, (sockaddr *) &_socketAddress, _addressLen) < 0)
		std::cout << "Error connect socket to adress" << std::endl;
		
	if (listen(_socket, 20) < 0 )
		std::cout << "Error listen" << std::endl;

	if (fcntl(_socket, F_SETFL, O_NONBLOCK) < 0) 
       std::cout << "Error listen" << std::endl;
	
	std::cout << "listening: address " << inet_ntoa(_socketAddress.sin_addr) << " port " << ntohs(_socketAddress.sin_port) << std::endl;

	fds[0].fd = _socket;
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
			std::cout<<"holaalal"<<std::endl;
			_acceptSocket = accept(_socket, (sockaddr *) &_socketAddress, &_addressLen);
			if (_acceptSocket < 0)
				std::cout << "Error accepting" << std::endl;
			std::cout<<_acceptSocket<<std::endl;
			//Assigning the new Client socket to the pollfd list that is empty
			for (size_t i = 1; i < fds.size(); i++)
			{
				if (fds[i].fd == 0)
				{
					fds[i].fd = _acceptSocket;
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
					bytesSent = write(_acceptSocket, response.getResponse().c_str(), response.getResponse().size());
					if (bytesSent < 0)
						std::cout << "Error writing" << std::endl;
				}
			}
		}
	}
	close(_socket);
}