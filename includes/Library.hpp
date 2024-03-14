#ifndef LIBRARY_HPP
# define LIBRARY_HPP
// Extern library
# include <netinet/in.h>
# include <arpa/inet.h>
# include <iostream>
# include <sys/socket.h>
# include <fcntl.h>
# include <sys/poll.h>
# include <unistd.h>
# include <fstream>
# include <string>
# include <map>
# include <sstream>
# include <algorithm>
# include <vector>
// Our library
# include "Request.hpp"
# include "Location.hpp"
# include "Config.hpp"
# include "Server.hpp"
# include "ConfigParser.hpp"
# include "Response.hpp"
# include "CgiHandler.hpp"
int manageServers(std::vector <Config> config);
#endif