#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include "Location.hpp"
#include "ErrorPage.hpp"

class Config
{
	private:
		std::string host;
		std::string	*serverNames;
		int 		port;
		std::string	bodySize;
		std::string root;
		std::string index;
		ErrorPage	*errors;
		Location 	*locations;
	public:
		Config();
		~Config();
		std::string	getHost();
		int			getPort();
		std::string	getRoot();
		std::string	getIndex();
		std::string	*getServerNames();
		ErrorPage	*getErrorPages();
		Location	*getLocations();
};
#endif