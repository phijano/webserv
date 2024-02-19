#ifndef LOCATION_H
#define LOCATION_H

#include <string>

class Location
{
	public:

	private:
		std::string route;
		std::string root;
		std::string index;
		std::string cgi_ext;
		std::string *allow_methods;
		std::string uploaded_path;
		bool	autoIndex;
		bool	allowUploads;
};
#endif