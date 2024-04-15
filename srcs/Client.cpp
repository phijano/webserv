#include "Client.hpp"

Client::Client():
	id(0),    
	config()
{}

Client::Client(Config config, int newClient, unsigned long clientIp):
	id(newClient),
    config(config),
	ip(clientIp)
{}

Client::~Client(){}

int	Client::getId()
{
	return (this->id);
}

Config	&Client::getConfig()
{
	return (this->config);
}

void	Client::setRequest(Request request)
{
	request.setClientIp(ip);
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