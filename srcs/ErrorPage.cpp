#include "ErrorPage.hpp"

ErrorPage::ErrorPage(){}
ErrorPage::~ErrorPage(){}

std::string	ErrorPage::getPath() {
    return this->path;
}

int	ErrorPage::getCode() {
    return this->code;
}