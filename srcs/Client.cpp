#include "Client.hpp"

Client::Client():
	fd(0),    
	config()
{}

Client::Client(Config config, int newClient):
	fd(newClient),
    config(config)
{}

Client::~Client(){}

int	Client::getId()
{
	return (this->fd);
}

Config	&Client::getConfig()
{
	return (this->config);
}

void	Client::setRequest(Request request)
{
	this->request = request;
}

Request &Client::getRequest()
{
	return (this->request);
}

void	Client::setTime(clock_t time)
{
	this->time = time;
}

clock_t &Client::getTime()
{
	return (this->time);
}

void	Client::remove()
{
	close(this->fd);
}