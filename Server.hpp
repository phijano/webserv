/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phijano- <phijano-@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/12 10:08:46 by phijano-          #+#    #+#             */
/*   Updated: 2024/02/09 12:17:34 by phijano-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
 #define SERVER_HPP

#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include "Request.hpp"

class Server
{
	public:

		Server();
		Server(std::string ip, int port);
		Server(const Server &other);
		Server &operator=(const Server &other);
		~Server();
	
		std::string getMethod(std::string path);	
		std::string postMethod(std::string path, std::vector<std::vector<std::string> > parameters);
		std::string deleteMethod(std::string path);

	private:

		std::string	_ip;
		int			_port;
		int			_socket;
		// short sin_family; unsigned short sin_port; struct in_addr {unsigned long s_addr}; char sin_zero; 
		sockaddr_in _socketAddress;
		int			_acceptSocket;
		socklen_t	_addressLen;
};

#endif
