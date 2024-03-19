#include "Response.hpp"

Response::Response()
{
}

Response::Response(Request& request, Config& config)
{
	_protocol = "HTTP/1.1";
	if (request.getError())
		getErrorPage(config, "400");
	else
	{
		_location = getRequestLocation(request, config);
		if (request.getMethod() == "GET")
		{
			if (isAllowedMethod("GET"))
				getMethod(request, config);
			else
				getErrorPage(config, "405");
		}
		else if (request.getMethod() == "POST")
		{
			if (isAllowedMethod("POST"))
				postMethod(request, config);
			else
				getErrorPage(config, "405");
		}
		else if (request.getMethod() == "DELETE")
		{
			if (isAllowedMethod("DELETE"))
				deleteMethod(request, config);
			else
				getErrorPage(config, "405");
		}
		else
			getErrorPage(config, "501");
	}
}
Response::Response(const Response& other)
{
	*this = other;
}

Response& Response::operator=(const Response& other) // Doesnt work because of private attributes
{
	_protocol = other._protocol;
	_code = other._code;
	_mime = other._mime;
	_body = other._body;

	return *this;
}

Response::~Response()
{
	
}

bool Response::isAllowedMethod(const std::string& method)
{
	const std::vector<std::string>& methods = _location.getAllowedMethods();
	for (size_t i = 0; i < methods.size(); ++i)
	{
		if (method == methods[i])
			return true;
	}
	return false;
}

std::string Response::getResponse() const
{
	std::stringstream response;

	if (!_cgiResponse.empty())
		return _cgiResponse;

	response << _protocol << " " << _code;
	
	if (!_mime.empty())
		response << "\nContent-Type: " << _mime << "\nContent-Length: " << _body.size() << "\n\n" << _body;
	return response.str();
}

void	Response::setCode(const std::string& code) // add more codes as we need
{
	switch(atoi(code.c_str()))
	{
		case 200:
			_code = "200 OK";
			break;
		case 201:
			_code = "201 Created";
			break;
		case 204:
			_code = "204 No Content";
			break;
		case 400:
			_code = "400 Bad Request";
			break;
		case 404:
			_code = "404 Not Found";
			break;
		case 405:
			_code = "405 Method Not Allowed";
			break;
		case 409:
			_code = "409 Conflict";
			break;
		case 500:
			_code = "500 Internal Server Error";
			break;
		case 501:
			_code = "501 Not Implemented";
			break;
		default:
			break;
	}
}

std::string Response::getExtension(const std::string& file)
{
	std::string ext;
	size_t pointPos;

	pointPos = file.find_last_of(".");
	if (pointPos != std::string::npos)
		ext = file.substr(file.find_last_of("."), file.size());
	return ext;
}

void Response::setMime(const std::string& file)//no idea how many to put here
{
	std::string ext;

	ext = getExtension(file);
	if (ext == ".htm" or ext == ".html")
		_mime = "text/html";
	else if (ext == ".txt")
		_mime = "text/plain";
	else if (ext == ".css")
		_mime = "text/css";
	else if (ext == ".gif")
		_mime = "image/gif";
	else if (ext == ".ico")
		_mime = "image/vnd.microsoft.icon";
	else if (ext == ".jar")
		_mime = "application/java-archive";
	else if (ext == ".jpeg" or ext == ".jpg")
		_mime = "image/jpeg";
	else if (ext == ".js")
		_mime = "text/javascript";
	else if (ext == ".png")
		_mime = "image/png";
	else
		_mime = "application/octet-stream";
}

Location Response::getRequestLocation(const Request& request, Config& config)
{
    std::vector<Location> locations = config.getLocations();
    Location mostSpecificLocation;
    size_t longestMatchLength = 0;

    for (size_t i = 0; i < locations.size(); ++i) 
	{
        const std::string& route = locations[i].getRoute();
        if (request.getPath().compare(0, route.length(), route) == 0)
		{
            if (route.length() > longestMatchLength)
			{
                mostSpecificLocation = locations[i];
                longestMatchLength = route.length();
            }
        }
    }
    // Returns NULL if no matching location is found, or pointer to the most specific matching location
    return mostSpecificLocation;
}


void Response::getErrorPage(const Config& config, const std::string error)
{
	std::stringstream resource;
	std::string path;

	setCode(error);
	if (config.getErrorPages().count(atoi(error.c_str())) > 0)
	{
		path = config.getRoot() + config.getErrorPages()[atoi(error.c_str())];
		std::cout << "Error page path: " << path <<std::endl;
		std::ifstream file(path);
		if (!file.is_open())
		{
			std::cerr << "Error: Failed to open file " << path << std::endl;
			//handle error, return error page
		}
		else
		{
			std::cout << "error page opened" << std::endl;
			setMime(path);
			resource << file.rdbuf();
			_body = resource.str();
			return;
		}
	}
	_mime = "text/html";
	_body = "<!DOCTYPE html><html lang=\"en\"><body><h1> " + _code + " </h1><p> Whooops! </p></body></html>";
}

std::string Response::getPath(const Request& request, const Config& config)
{
	std::string path;

	if (!_location.getRoot().empty())
		path = _location.getRoot();
	else
		path = config.getRoot();
	path += request.getPath();
	return path;
}

std::string Response::getIndex(const Config& config)
{
	std::string index;

	if (!_location.getIndex().empty())
		index = _location.getIndex();
	else
		index = config.getIndex();
	return index;
}


void Response::getMethod(const Request& request, const Config& config)
{
    std::string file = request.getFile();
    std::string path = getPath(request, config);

    if (file.empty())
        file = getIndex(config);

	if (!_location.getCgiExt().empty() && _location.getCgiExt() == getExtension(file))
	{
		if (!_location.getCgiPath().empty())
			path = _location.getCgiPath();
		CgiHandler cgi(request, config, path);
		if (cgi.getError().empty())
			_cgiResponse = cgi.getResponse();
		else
			getErrorPage(config, cgi.getError());
	}
    std::ifstream fileStream((path + file).c_str());
    if (fileStream.is_open())
	{
        std::stringstream buffer;
        buffer << fileStream.rdbuf();
        _body = buffer.str();
        setCode("200");
        setMime(file);
	}
    else
        getErrorPage(config, "404");
}

// Function to write file contents to a new file
void writeFile(const std::string& filePath, const std::string& content) 
{
    std::ofstream outputFile(filePath.c_str(), std::ios::binary);
    if (outputFile) 
	{
        outputFile << content;
        outputFile.close();
    } 
	else
        std::cerr << "Error writing file." << std::endl;
}

void Response::postMethod(const Request& request, const Config& config) 
{
    if (!request.getBody().empty()) 
	{
        std::string body = request.getBody();
        std::string name;
        std::string originalFilePath;

        size_t lastEqualsPos = body.rfind('=');
        if (lastEqualsPos != std::string::npos) 
		{
            originalFilePath = "../" + body.substr(lastEqualsPos + 1); // hardcoded

            size_t firstEqualsPos = body.find('=');
            size_t ampersandPos = body.find('&');
            if (firstEqualsPos != std::string::npos && ampersandPos != std::string::npos && !originalFilePath.empty())
			{
                name = body.substr(firstEqualsPos + 1, ampersandPos - firstEqualsPos - 1);
				if (name.empty())
					name = body.substr(lastEqualsPos + 1);
                char cwd[1024];
    			getcwd(cwd, sizeof(cwd));
				std::string	dir(cwd);
				std::string filePath;
				if (request.getPath() == "/")
					filePath = dir + "/" + name;
				else
					filePath = dir + request.getPath() + "/" + name;
                std::ifstream originalFile(originalFilePath.c_str(), std::ios::binary);
                if (originalFile)
				{
					std::stringstream buffer;
                    buffer << originalFile.rdbuf();
                    std::string fileContent = buffer.str();
                    writeFile(filePath, fileContent);
					std::string path = getPath(request, config);
					std::cout << "Path: " << path << std::endl;
					std::string file = request.getFile();
					std::cout << "Request: " << request.getFile() << std::endl;
					if (file.empty())
						file = getIndex(config);
					std::ifstream fileStream((path + file).c_str());
    				if (fileStream.is_open())
					{
       					std::stringstream buffer;
        				buffer << fileStream.rdbuf();
        				_body = buffer.str();
        				setCode("200");
        				setMime(file);
					}
                } 
				else 
				{
                    getErrorPage(config, "204"); // All error pages are random
                    // Error opening the original file, send appropriate error response
                }
            } 
			else 
			{
                getErrorPage(config, "405");
                // '=' or '&' not found in the request body, send appropriate error response
            }
        } 
		else 
		{
            getErrorPage(config, "404");
            // '=' not found in the request body, send appropriate error response
        }
    } 
	else 
	{
        getErrorPage(config, "500");
        // No body present in the request, send appropriate error response
    }
}


void Response::deleteMethod(const Request& request, const Config& config)
{
	std::string path = getPath(request, config);
	if (request.getFile().empty())
		setCode("noidea"); // Change to error code
	path += request.getFile();
	std::remove(path.c_str());
	setCode("204");
	//check for errors to send correct error page like not allowed
}
