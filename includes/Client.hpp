#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "Library.hpp"
# include "Config.hpp"
# include "Request.hpp"

class Client
{
	private:
		int	fd;
		Request request;
		Config config;
		clock_t time;

	public:
		Client();
		Client(Config config, int newClient);
		~Client();
		Request &getRequest();
		void	setRequest(Request request);
		int	getId();
		void	setTime(clock_t);
		void	remove();
		clock_t &getTime();
		Config &getConfig();

};

#endif
