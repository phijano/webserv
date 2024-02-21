#ifndef CONFIG_H
# define CONFIG_H

# include <string>
# include <vector>
# include "Location.hpp"
# include "ErrorPage.hpp"

class Config
{
	public:
		Config();
		~Config();

		//Getters
		std::string					getHost();
		int							getPort();
		std::string					getRoot();
		std::string					getIndex();
		std::vector<std::string>	getServerNames();
		std::vector<ErrorPage>		getErrorPages();
		std::vector<Location>		getLocations();

		//Setters
		void	setHost(const std::string newHost);
		void	addServerName(std::string newServerName);
		void 	setPort(int newPort);
		void 	setBodySize(const std::string newSize);
		void 	setRoot(const std::string newRoot);
		void 	setIndex(const std::string newIndex);
		void 	addErrorPage(ErrorPage newError);
		void 	addLocation(Location newLocation);


	friend std::ostream& operator<<(std::ostream& os, const Config& config);

	private:
		std::string 				host; // Should host be string or other type ???
		std::vector<std::string>	serverNames;
		int 						port;
		std::string					bodySize;
		std::string 				root;
		std::string 				index;
		std::vector<ErrorPage>		errors;
    	std::vector<Location>		locations;
};

#endif