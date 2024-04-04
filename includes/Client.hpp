#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "Library.hpp"
# include "Config.hpp"
# include "Request.hpp"

class Client
{
	private:
		int	id;
		Request request;
		Config *config;
		unsigned long ip;

	public:
		Client();
		Client(Config *config, int newClient, unsigned long ip);
		~Client();
		Request &getRequest();
		void	setRequest(Request request);
		int	getId();
		Config &getConfig();

};

#endif
