#ifndef LOCATION_H
#define LOCATION_H

#include <string>

class Location
{
	private:
		std::string route;
		std::string root;
		std::string index;
		std::string cgiExt;
		std::string *allowMethods;
		std::string uploadedPath;
		bool	autoIndex;
		bool	allowUploads;
	public:
		Location();
		~Location();
		std::string getRoute();
		std::string getRoot();
		std::string getIndex();
		std::string getCgiExt();
		std::string *getAllowMethods();
		std::string getUploadedPath();
		bool	getAutoIndex();
		bool	getAllowUploads();
};
#endif