#include "Library.hpp"

void addClient(std::vector <pollfd> conn, std::vector <Client> client)
{
	int acceptSocket;

	if (fcntl(acceptSocket, F_SETFL, O_NONBLOCK) < 0)
		std::cout<<"Error"<<std::endl;
	acceptSocket = accept(serverSocket, (struct sockaddr *)&serverAddress, &addressLen);
	conn.push_back(client.id);
}

int manageServers(std::vector <Config> config)
{
	std::vector <Server> servers;
	std::vector <Client> clients;
	std::vector <pollfd> conn;
	int	activity;

	//CREATE AND ASSIGN SERVERS TO THEIR RESPECTIVES FD
	for (size_t i = 0; i < config.size(); i++)
	{
		conn[i].fd = servers[i].getServerSocket();
		conn[i].events = POLLIN;
		servers[i].setServerConfig(&config[i]);
	}

	while (1)
	{
		activity = poll(conn.data(), conn.size(), -1);
		//CHECK IF ONE OF THE SERVERS HAVE AN EVENT
		for (size_t i = 0; i < config.size(); i++)
		{
			
		}
	}

	return 0;
}
