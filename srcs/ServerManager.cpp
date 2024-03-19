#include "ServerManager.hpp"

ServerManager::ServerManager()
{}

ServerManager::ServerManager(std::vector<Config> configs)
{
	Server a = Server();
	this->servers.push_back(a);
	this->servers.push_back(Server());
	this->servers.push_back(Server());
	// for (size_t i = 0; i < configs.size(); i++)
	// {
	// 	Server *ser = new Server(&configs[i]);
	// 	// servers.push_back(*ser);
	// }
	
}

ServerManager::~ServerManager(){}