#ifndef ERRORPAGE_H
#define ERRORPAGE_H

#include <string>

class ErrorPage
{
	private:
		std::string path;
		int	code;
	public:
		ErrorPage();
		~ErrorPage();
		std::string getPath();
		int			getCode();
	
};
#endif