#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include "Location.hpp"
#include "ErrorPage.hpp"

class Config
{
	public:

	private:
		std::string _host;
		std::string	*_serverNames;
		int 		_port;
		std::string	_bodySize;
		std::string root;
		std::string index;
		ErrorPage	*errors;
		Location 	*locations;
};
#endif