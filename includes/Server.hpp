/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbengoec <pbengoec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/12 10:08:46 by phijano-          #+#    #+#             */
/*   Updated: 2024/02/22 20:01:39 by pbengoec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <netinet/in.h>
# include <arpa/inet.h>
#include <fcntl.h>
# include <sys/poll.h>
# include "Request.hpp"
# include "Response.hpp"
# include "Config.hpp"

class Server
{
	private:
		Config		*config;
		int			serverSocket;
		sockaddr_in *serverAddress;
		int			acceptSocket;
		socklen_t	addressLen;

	public:
		Server();
		Server(Config *config);
		Server(const Server &other);
		~Server();
		void	initServer(void);
};

#endif
