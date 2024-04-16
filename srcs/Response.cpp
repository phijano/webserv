/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbengoec <pbengoec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 18:46:28 by vnaslund          #+#    #+#             */
/*   Updated: 2024/04/16 20:25:51 by pbengoec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

Response::Response()
{
}

bool	Response::wrongHost(std::string host, std::vector<std::string> validHosts, Config config)
{
	std::vector<std::string>::iterator it;

	if (host == "localhost" || host == config.getHost())
		return false;
    for (it = validHosts.begin(); it != validHosts.end(); ++it)
    {
        if (host == *it)
            return false;
    }
    return true;
}

Response::Response(Request& request, Config& config)
{
	_protocol = "HTTP/1.1";
	_indexNotFound = false;
	if (request.getError() || wrongHost(request.getHost(), config.getServerNames(), config))
		getErrorPage(config, "400");
	else if (!request.getContentLength().empty() and atoi (request.getContentLength().c_str()) > config.getBodySize())
		getErrorPage(config, "413");
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
	_cgiResponse = other._cgiResponse;
	return *this;
}

Response::~Response()
{
	
}

std::string	Response::createIndex(std::string fullPath, std::string path)
{
	DIR *dir;
	struct dirent *entry;
	std::string dirPath;
	std::string html;
	std::string title;
	std::string body;

	dir = opendir(fullPath.c_str());
	html = "<!DOCTYPE html><html lang='en'><head><meta charset='UTF-8'><meta name='viewport' content='width=device-width, initial-scale=1.0'><title>Index</title><link rel='stylesheet' href='../assets/styles/style.css'></head>";
	body = "<body><h1>Index of " + path + "</h1><hr>";
	if (!dir)
	{
		body += "</body></html>";
		html += body;
		return html;
	}
	while ((entry = readdir(dir)) != NULL) 
	{ // Leer todas las entradas del directorio
		if (path[path.length() - 1] == '/')
			body += "<a href='"  + path + entry->d_name +"'>"+entry->d_name+"</a><br>";
		else
			body += "<a href='"  +path + "/" + entry->d_name +"'>"+entry->d_name+"</a><br>";
    }
	body += "</body></html>";
	html += body;
	closedir(dir);
	return (html);
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
		response << "\nContent-Type: " << _mime <<"\nContent-Length: "<< _body.size() << "\n\n" << _body;
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
		case 403:
			_code = "403 Forbidden";
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
		case 413:
			_code = "413 Content Too Large";
			break;
		case 500:
			_code = "500 Internal Server Error";
			break;
		case 501:
			_code = "501 Not Implemented";
			break;
		case 508:
			_code = "508 Loop Detected";
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
        const std::string& path = locations[i].getPath();
        if (request.getPath().compare(0, path.length(), path) == 0)
		{
            if (path.length() > longestMatchLength)
			{
                mostSpecificLocation = locations[i];
                longestMatchLength = path.length();
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
		std::cout << "Error page path: " << path << std::endl;
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
	_body = "<!DOCTYPE html><html lang=\"en\"><head><title>Error</title><link rel='stylesheet' href='../assets/styles/style.css'><head><body><h1> " + _code + " </h1><p style='text-align: center;'> Whooops! </p></body></html>";
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
	else if (!config.getIndex().empty())
		index = config.getIndex();
	else
	{
		_indexNotFound = true;
		index = "";
	}
	return index;
}


void Response::getMethod(const Request& request, const Config& config)
{
	std::string path = getPath(request, config);

    std::string file = request.getFile();
    if (file.empty())
        file = getIndex(config);

	if (!_location.getCgiExt().empty() && _location.getCgiExt() == getExtension(file) && !_indexNotFound)
	{
		if (!_location.getCgiPath().empty())
			path += _location.getCgiPath();
		CgiHandler cgi(request, config, path);
		if (cgi.getError().empty())
			_cgiResponse = cgi.getResponse();
		else
			getErrorPage(config, cgi.getError());
		return;
	}
	std::string fullPath = path + file;
	if (!_indexNotFound)
	{
    	std::ifstream fileStream((fullPath).c_str());
    	if (fileStream.is_open())
		{
        	std::stringstream buffer;
        	buffer << fileStream.rdbuf();
        	_body = buffer.str();
        	setCode("200");
        	setMime(file);
			return ;
		}
		else
			getErrorPage(config, "404");
	}
    else if (_location.getAutoIndex() && access(fullPath.c_str() , F_OK) == 0)
	{
        _body = createIndex(fullPath, request.getPath());
        setCode("200");
        setMime(".html");
		return ;
	}
	else if (!_location.getAutoIndex() && access(fullPath.c_str() , F_OK) == 0)
		getErrorPage(config, "403");
	else
        getErrorPage(config, "404");
}

// Function to write file contents to a new file
int	Response::createFile(const std::string& file, const std::string& content, const Config& config) 
{
    std::ofstream outputFile(file.c_str(), std::ios::binary);
    if (outputFile) 
	{
        outputFile << content;
        outputFile.close();
		return (0);
    } 
	else
	{
        getErrorPage(config, "409");
		return (1);
	}
}

void Response::upload(const Request& request, const Config& config) 
{
    if (!request.getBody().empty()) 
	{
        std::string body = request.getBody();
        std::string name;
        std::string originalFilePath;

        size_t lastEqualsPos = body.rfind('=');
        if (lastEqualsPos != std::string::npos) 
		{
            originalFilePath = config.getUploadDir() + "/" + body.substr(lastEqualsPos + 1);

            size_t firstEqualsPos = body.find('=');
            size_t ampersandPos = body.find('&');
            if (firstEqualsPos != std::string::npos && ampersandPos != std::string::npos && !originalFilePath.empty())
			{

                name = body.substr(firstEqualsPos + 1, ampersandPos - firstEqualsPos - 1);
				if (name.empty()) // if no name specified, use og name
					name = body.substr(lastEqualsPos + 1);
				std::string newfilePath;
				if (!_location.getUploadPath().empty())
					newfilePath = getPath(request, config) + _location.getUploadPath() + "/" + name;
				else
				{
					if (_location.getPath() == "/")
						newfilePath = getPath(request, config) + name;
					else 
						newfilePath = getPath(request, config) + "/" + name;
				}
                std::ifstream originalFile(originalFilePath.c_str(), std::ios::binary);
                if (originalFile)
				{
					std::stringstream buffer;
                    buffer << originalFile.rdbuf();
                    std::string fileContent = buffer.str();
                    if (createFile(newfilePath, fileContent, config) == 1)
						return ;
					std::string path = getPath(request, config);
					std::string file = request.getFile();
					if (file.empty())
						file = getIndex(config);
					if (file.empty() && _location.getAutoIndex())
					{
						_body = createIndex(path, request.getPath());
        				setCode("200");
        				setMime(".html");
						return ;
					}
					else if (file.empty())
					{
						getErrorPage(config, "403");
						return ;
					}
					std::string	newIndex;
					if (_location.getPath() == "/")
						newIndex = path + file;
					else
						newIndex = path + "/" + file;
					std::ifstream fileStream((newIndex).c_str());
    				if (fileStream.is_open())
					{
       					std::stringstream buffer;
        				buffer << fileStream.rdbuf();
        				_body = buffer.str();
        				setCode("200");
        				setMime(file);
					}
					else
					{
                    	getErrorPage(config, "404");
						// newFile was not created
                	}
                } 
				else
				{
                    getErrorPage(config, "404");
                    // File can not be opened (probably not in upload_dir), send appropriate error response
                }
            } 
			else 
			{
                getErrorPage(config, "400");
                // '=' or '&' not found in the request body, send appropriate error response
            }
        } 
		else 
		{
            getErrorPage(config, "400");
            // '=' not found in the request body, send appropriate error response
        }
    } 
	else 
	{
        getErrorPage(config, "400");
        // No body present in the request, send appropriate error response
    }
}

void Response::postMethod(const Request& request,const Config& config)//Dont know what response send if no files send only fields
{
	std::string file = request.getFile();
	std::string path = getPath(request, config);

	if (!_location.getCgiExt().empty() && _location.getCgiExt() == getExtension(file))
	{
		if (!_location.getCgiPath().empty())
			path += _location.getCgiPath();
		CgiHandler cgi(request, config, path);
		if (cgi.getError().empty())
			_cgiResponse = cgi.getResponse();
		else
			getErrorPage(config, cgi.getError());
		return;
	}
	upload(request, config);
}



void Response::deleteMethod(const Request& request, const Config& config)
{
	std::string delPath = getPath(request, config);
	std::string delFile = request.getFile();
	std::string filePath = delPath + delFile;
	if (std::remove(filePath.c_str()) != 0)
	{
		getErrorPage(config, "404");
		return ;
	}
	std::string path = _location.getRoot() + "/";
	std::string	file = _location.getIndex();
	if (file.empty() && _location.getAutoIndex())
	{
		_body = createIndex(path, request.getPath());
        setCode("200");
        setMime(".html");
		return ;
	}
	else if (file.empty())
	{
		getErrorPage(config, "403");
		return ;
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
		getErrorPage(config, "409");
}
