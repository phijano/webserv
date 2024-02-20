#include "ErrorPage.hpp"

ErrorPage::ErrorPage():
path(""),
code(0)
{
}
ErrorPage::ErrorPage(std::string path, int code)
{
	setPath(path);
	setCode(code);
}
ErrorPage::~ErrorPage(){}

std::string	ErrorPage::getPath() {
    return this->path;
}

int	ErrorPage::getCode() {
    return this->code;
}

void	ErrorPage::setPath(std::string newPath)
{
	this->path = newPath;
}

void	ErrorPage::setCode(int	newCode)
{
	this->code = newCode;
}