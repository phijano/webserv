#ifndef SERVICE_HPP
# define SERVICE_HPP

# include "Library.hpp"

class Service
{
	private:
		std::vector <pollfd> conn;
		std::vector <Server> servers;

	public:
		Service();
		Service(std::vector <Config> config);

};

#endif
