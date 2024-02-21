#ifndef LOCATION_H
# define LOCATION_H

# include <string>
# include <iostream>

class Location
{
	public:
		Location();
		~Location();

		//Getters
		std::string getRoute();
		std::string getRoot();
		std::string getIndex();
		std::string getCgiExt();
		std::string *getAllowMethods();
		std::string getUploadedPath();
		bool	getAutoIndex();
		bool	getAllowUploads();
		//Setters

		void setRoute(const std::string newRoute);
		void setRoot(const std::string newRoot);
		void setIndex(const std::string newIndex);
		void setCgiExt(const std::string newCgiExt);
		void setAllowMethods(std::string* newAllowMethods);
		void setUploadedPath(const std::string newUploadedPath);
		void setAutoIndex(bool newAutoIndex);
		void setAllowUploads(bool newAllowUploads);
	
	friend std::ostream& operator<<(std::ostream& os, const Location& location);

	private:
		std::string route;
		std::string root;
		std::string index;
		std::string cgiExt;
		std::string *allowMethods;
		std::string uploadedPath;
		bool	autoIndex;
		bool	allowUploads;
};

#endif