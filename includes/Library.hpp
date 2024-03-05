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
# include "Location.hpp"
# include "Request.hpp"
# include "Config.hpp"
# include "Response.hpp"
# include "CgiHandler.hpp"
class Request;
class Config;
class Location;
class CgiHandler;
#endif