#include "Client.hpp"

Client::Client():
	id(0),    
	config()
{}

Client::Client(Config *config, int newClient):
	id(newClient),
    config(config)
{}

Client::~Client(){}

int	Client::getId()
{
	return (this->id);
}

Config	&Client::getConfig()
{
	return (*this->config);
}

void	Client::setRequest(Request request)
{
	this->request = request;
}

Request &Client::getRequest()
{
	return (this->request);
}