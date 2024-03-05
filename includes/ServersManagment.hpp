#ifndef SERVERSMANAGMENT_HPP
# define SERVERSMANAGMENT_HPP
# include "Library.hpp"
class ServersManagment
{
	private:
		Server		*servers;
		int			serverSocket;
		sockaddr_in serverAddress;
		socklen_t	addressLen;
		void		initServer(void);
		void		connectServerAddress(void);

	public:
		ServersManagment();
		Server(Config *config);
		Server(const Server &other);
		~ServersManagment();
		void	setServerAddress(Config *config);
};

#endif
