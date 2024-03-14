#include "Library.hpp"

int manageServers(std::vector <Config> config)
{
	std::vector <Server> servers;

	//CREATE AND ASSIGN SERVERS TO THEIR RESPECTIVES FD
	for (size_t i = 0; i < config.size(); i++)
	{
		files[i].fd = servers[i].getServerSocket();
		files[i].events = POLLIN;
		servers[i].setServerConfig(&config[i]);
	}

	while (1)
	{
		activity = poll(files.data(), files.size(), -1);
		//CHECK IF ONE OF THE SERVERS HAVE AN EVENT
		for (size_t i = 0; i < config.size(); i++)
		{
		}
	}

	return 0;
}
