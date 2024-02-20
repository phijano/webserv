#ifndef ERRORPAGE_H
#define ERRORPAGE_H

#include <string>

class ErrorPage
{
	private:
		std::string path;
		int	code;
	public:
		//Constructors
		ErrorPage();
		ErrorPage(std::string path, int code);
		~ErrorPage();
		//Getters
		std::string getPath();
		int			getCode();
		//Setters
		void	setPath(std::string newPath);
		void	setCode(int	newCode);
	
};
#endif