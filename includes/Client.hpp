#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "Library.hpp"

class Client
{
	private:
		int	id;
		Config *config;

	public:
		Client();
		Client(Config *config, int newClient);
		~Client();

};

#endif
