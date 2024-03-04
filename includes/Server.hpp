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
# include "Config.hpp"

class Server
{
	private:
		Config		*config;
		int			serverSocket;
		sockaddr_in serverAddress;
		socklen_t	addressLen;
		void		initServer(void);
		void		connectServerAddress(void);

	public:
		Server();
		Server(Config *config);
		Server(const Server &other);
		~Server();
		void	setServerAddress(Config *config);
};

#endif
