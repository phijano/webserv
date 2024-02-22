/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbengoec <pbengoec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/12 10:08:46 by phijano-          #+#    #+#             */
/*   Updated: 2024/02/22 18:20:58 by pbengoec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <netinet/in.h>
# include <arpa/inet.h>
# include <iostream>
# include <sys/socket.h>
#include <fcntl.h>
# include <sys/poll.h>
# include "Request.hpp"
# include "Response.hpp"

class Server
{
	private:
		std::string	_ip;
		int			_port;
		int			_socket;
		sockaddr_in _socketAddress;
		int			_acceptSocket;
		socklen_t	_addressLen;

	public:
		Server();
		Server(std::string ip, int port);
		Server(const Server &other);
		Server &operator=(const Server &other);
		~Server();
		void	initServer(void);
};

#endif
