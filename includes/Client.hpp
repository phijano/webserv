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

	public:
		Client();
		Client(Config *config, int newClient);
		~Client();
		Request &getRequest();
		void	setRequest(Request request);
		int	getId();
		Config &getConfig();

};

#endif
