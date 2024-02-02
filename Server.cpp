/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phijano- <phijano-@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/12 10:10:40 by phijano-          #+#    #+#             */
/*   Updated: 2024/02/02 13:00:12 by phijano-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <fstream>
#include <sstream>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

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
	std::cout << "Server constructor called" << std::endl;

	_socketAddress.sin_family = PF_INET;
	_socketAddress.sin_port = htons(_port);
	_socketAddress.sin_addr.s_addr = inet_addr(_ip.c_str());
	
	_socket = socket(PF_INET, SOCK_STREAM, 0);
	if (_socket < 0)
	{
		//handle error
		std::cout << "Error socket" << std::endl;
	}
	
	if (bind(_socket, (sockaddr *) &_socketAddress, _addressLen) < 0)
	{
		//handle error
		std::cout << "Error connect socket to adress" << std::endl;
	}

	//listen(socket, max_number_of_connections)
	if (listen(_socket, 20) < 0 )
	{
		//handle error
		std::cout << "Error listen" << std::endl;
	}
	std::cout << "listening: address " << inet_ntoa(_socketAddress.sin_addr) << " port " << ntohs(_socketAddress.sin_port) << std::endl;

	// fd, buffer, buffer size,
	char buffer[30720];
	long bytes;

	while (true)
	{
		_acceptSocket = accept(_socket, (sockaddr *) &_socketAddress, &_addressLen);

		if (_acceptSocket < 0)
		{
			//handle error
			std::cout << "Error accepting" << std::endl;
		}

		bytes = read(_acceptSocket, buffer, 30720); 
		if (bytes < 0)
		{
			std::cout << "Error reading " << strerror(errno) << std::endl;
		}
		std::cout << "request bytes " << bytes << std::endl << buffer << std::endl;


		std::string resource = getMethod("testweb/index.html");
		//std::string resource = getMethod("erewrer");
		
		std::ostringstream ss; 
		ss << resource;

		long bytesSent;
		//fcntl(_acceptSocket, F_SETFL, O_NONBLOCK, FD_CLOEXEC); I dont know whats is this for
		bytesSent = write(_acceptSocket, ss.str().c_str(), ss.str().size());
		if (bytesSent < 0)
		{
			std::cout << "Error writing" << std::endl;
		}

		close(_acceptSocket);
	}
}

Server::Server(const Server& other)
{
	std::cout << "Server copy constructor called" << std::endl;
	*this = other;
}

Server& Server::operator=(const Server& other)
{
	std::cout << "Server assignation operator called" << std::endl;
	_ip = other._ip;
	_port = other._port;
	_socket = other._socket;
	return *this;
}

Server::~Server()
{
	std::cout << "Server destructor called" << std::endl;
	close(_socket);
}

std::string Server::getMethod(std::string path)
{
	std::ifstream file(path);
	std::stringstream resource;
	std::stringstream response;

	if (file.is_open())
	{
		resource << file.rdbuf();
		response << "HTTP/1.1 " << "200 OK\n" << "Content-Type: text/html\nContent-Length: " << resource.str().size() << "\n\n" << resource.str();
	}
	else
	{
		std::cout << "Error, file not found" << std::endl;
		std::ifstream file("errorpages/404.html");
		if (file.is_open())
		{
			resource << file.rdbuf();
			response << "HTTP/1.1 " << "404 Not Found\n" << "Content-Type: text/html\nContent-Length: " << resource.str().size() << "\n\n" << resource.str();
		}
		else
			std::cout << "not error page" << std::endl; // no idea what response send here
	}
	return response.str();
}

std::string Server::postMethod(std::string path, std::string content)
{

	std::ifstream file(path);
	std::stringstream response;
	std::stringstream resource;

	if (file.good())
	{
		std::cout << "Error file already exist";// I dont know what happen with overwrite
		std::ifstream file("errorpages/409.html");
		if (file.is_open())
		{
			resource << file.rdbuf();
			response << "HTTP/1.1 " << "409 Conflict\n" << "Content-Type: text/html\nContent-Length: " << resource.str().size() << "\n\n" << resource.str();
		}
		else
			std::cout << "not error page" << std::endl; // no idea what response send here
	}
	else
	{
		std::ofstream newFile(path);
		newFile << content;
		newFile.close();
		response << "HTTP/1.1 " << "201 CREATED";
	}
	return response.str();
}

std::string Server::deleteMethod(std::string path)
{
	std::stringstream response;
	std::stringstream resource;

	std::remove(path.c_str());
	response << "HTTP/1.1 " << "204 No Content";
	//check for errors to send correct error page like not allowed
	return response.str();
}





