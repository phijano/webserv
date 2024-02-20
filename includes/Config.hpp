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
		//Constructor
		Config();
		~Config();
		//Getters
		std::string	getHost();
		int			getPort();
		std::string	getRoot();
		std::string	getIndex();
		std::string	*getServerNames();
		ErrorPage	*getErrorPages();
		Location	*getLocations();
		//Setters
		void setHost(const std::string newHost);
		void setServerNames(std::string* newServerNames);
		void setPort(int newPort);
		void setBodySize(const std::string newSize);
		void setRoot(const std::string newRoot);
		void setIndex(const std::string newIndex);
		void setErrors(ErrorPage* newErrors);
		void setLocations(Location* newLocations);
};
#endif