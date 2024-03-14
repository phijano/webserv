#ifndef SERVER_HPP
# define SERVER_HPP

# include "Library.hpp"

class Server
{
	private:
		Config		*config;
		int			serverSocket;
		sockaddr_in serverAddress;
		socklen_t	addressLen;
		void		initServer(void);
		void		connectServerAddress(void);
		void		setServerAddress(Config *config);

	public:
		Server();
		Server(Config *config);
		Server(const Server &other);
		~Server();
		int			getServerSocket();
		sockaddr_in	getServerAddress();
		socklen_t	getServerAddressLen();
		
		void setServerConfig(Config *config);
};

#endif
