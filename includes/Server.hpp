/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vnaslund <vnaslund@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/12 10:08:46 by phijano-          #+#    #+#             */
/*   Updated: 2024/02/16 18:08:27 by vnaslund         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <netinet/in.h>
# include <arpa/inet.h>
# include "Request.hpp"
# include "Response.hpp"

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