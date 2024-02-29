/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phijano- <phijano-@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 10:57:54 by phijano-          #+#    #+#             */
/*   Updated: 2024/02/29 11:20:44 by phijano-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

Response::Response()
{
}

Response::Response(Request request, Config config)
{
	std::cout << "sadas" << std::endl;
	_protocol = "HTTP/1.1";
	if (request.getError())
		getErrorPage(config, "400");
	else
	{
		_location = getRequestLocation(request, config);
		if (_location)
			std::cout << "Location selected" << _location->getRoute() << std::endl;
		if (request.getMethod() == "GET")
		{
			if (isAllowedMethod("GET"))
			{	
			std::cout << "GET" << std::endl;
			getMethod(request, config);
			}
			else
				getErrorPage(config, "405");
		}
		else if (request.getMethod() == "POST")
		{
			if (isAllowedMethod("POST"))
			{
				std::cout << "POST" << std::endl;
				postMethod(request, config);
			}
			else
				getErrorPage(config, "405");
		}
		else if (request.getMethod() == "DELETE")
		{
			if (isAllowedMethod("DELETE"))
			{
				std::cout << "DELETE" << std::endl;
				deleteMethod(request, config);
			}
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

Response& Response::operator=(const Response& other)
{
	_protocol = other._protocol;
	_code = other._code;
	_mime = other._mime;
	_body = other._body;

	return *this;
}

Response::~Response()
{
	if (_location)
		delete _location;
}

bool Response::isAllowedMethod(std::string method)
{
	if (_location)
	{
		std::vector<std::string> methods = _location->getAllowedMethods();
		for (std::vector<std::string>::iterator it = methods.begin(); it != methods.end();it++)
			if (method == *it)
				return true;
		return false;
	}
	return true;
}

std::string Response::getResponse() const
{
	std::stringstream response;

	//std::cout << "cgiresponse :" <<_cgiResponse << std::endl;;
	if (!_cgiResponse.empty())
	{
		//std::cout << "SEND" << std::endl;

		return _cgiResponse;
	}
	response << _protocol << " " << _code;
	if (_mime != "")
		response << "\nContent-Type: " << _mime << "\nContent-Length: " << _body.size() << "\n\n" << _body;
	return response.str();
}

void	Response::getCode(std::string code) // add more codes as we need
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
		case 501:
			_code = "501 Not Implemented";
			break;
		default:
			break;
	}
}

std::string Response::getExtension(std::string file)
{
	std::string ext;
	size_t pointPos;

	pointPos = file.find_last_of(".");
	if (pointPos != std::string::npos)
		ext = file.substr(file.find_last_of("."), file.size());
	return ext;
}

void Response::getMime(std::string file)//no idea how many to put here
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
	std::cout << "EXT: " << ext << "<-" << std::endl;
}

Location *Response::getRequestLocation(Request request, Config config)
{
	std::vector<Location> locations = config.getLocations();
	Location* loc = NULL;

	for (std::vector<Location>::iterator it = locations.begin(); it != locations.end(); it++)
	{
		if (it->getRoute() == request.getPath().substr(0, it->getRoute().size()))
		{
			std::cout << it->getRoute() << " == " << request.getPath().substr(0, it->getRoute().size()) << std::endl;
			std::cout << "Location found: " << it->getRoute() << std::endl;
			if (!loc or loc->getRoute().size() < it->getRoute().size())
			{
				std::cout << "assigned loc" << std::endl;
				loc = &*it;
			}
		}
	}
	loc = new Location(*loc);
	return loc;
}

void Response::getErrorPage(Config config, std::string error)
{
	std::stringstream resource;
	std::string path;

	getCode(error);
	if (config.getErrorPages().count(atoi(error.c_str())) > 0)
	{
		path = config.getRoot() + config.getErrorPages()[atoi(error.c_str())];
		std::cout << "Error page path: " << path <<std::endl;
		std::ifstream file(path);
		if (file.is_open())
		{
			std::cout << "error page opened" << std::endl;
			getMime(path);
			resource << file.rdbuf();
			_body = resource.str();
			return;
		}
	}
	_mime = "text/html";
	_body = "<!DOCTYPE html><html lang=\"en\"><body><h1> " + _code + " </h1><p> Whooops! </p></body></html>";
}

std::string Response::getPath(Request request, Config config)
{
	std::string path;

	if (_location and _location->getRoot() != "")
		path = _location->getRoot();
	else
		path = config.getRoot();
	path += request.getPath();	
	return path;
}

std::string Response::getIndex(Config config)
{
	std::string index;

	if (_location and _location->getIndex() != "")
		index = _location->getIndex();
	else
		index = config.getIndex();
	return index;
}


void Response::getMethod(Request request, Config config)
{
	std::stringstream resource;
	std::stringstream response;
	std::string file = request.getFile();
	std::string path = getPath(request, config);
	std::cout << "GET PATH: " << path << std::endl; 
	
	if (file == "")//autolisting ??
	{
		if (_location and !_location->getAutoIndex())
		{
			std::cout << "list directory" << std::endl;
			return;
		}
		else
			file = getIndex(config);
	}
	if (_location and _location->getCgiExt()!= "" and getExtension(file) == _location->getCgiExt())//cgi extension config file
	{
		if (_location->getCgiPath() != "")
			path = _location->getCgiPath();
		CgiHandler cgi(request, config, path);
		if (cgi.getError().empty())
			_cgiResponse = cgi.getResponse();
		else
			getErrorPage(config, cgi.getError());
	}
	else
	{
		std::ifstream fileStream(path + file);
		if (fileStream.is_open())
		{
			resource << fileStream.rdbuf();
			getCode("200");
			getMime(file);
			_body = resource.str();
		}
		else
			getErrorPage(config, "404");
	}
	std::cout << "END GET" << std::endl;
}

void Response::uploadFile(std::string path, std::string formField)
{
	std::stringstream ss(formField);
	std::string line;
	std::string fileName;
	std::string fileContent;
	size_t startFileName;
	size_t endFileName;

	getline(ss, line);
	startFileName = line.find("filename=\"");
	if (startFileName != std::string::npos)
	{
		endFileName = line.find("\"", startFileName + 10);
		fileName = line.substr(startFileName + 10, endFileName - startFileName - 10);
		std::cout << "filename: " << fileName << std::endl;
	}
	getline(ss, line);
	if (line != "\r")
		getline(ss, line);
	getline(ss, fileContent, '\r');
	std::cout << "Content: " << fileContent << "<-Content"<< std::endl;
	std::ifstream file(path + fileName); // path + filename
	if (!file.good())
	{
		std::ofstream newFile(path + fileName);
		newFile << fileContent; //file content
		newFile.close();
		getCode("201");
	}
}

void Response::staticPost(Request request, Config config)
{
	std::string path;
	if (_location->getUploadedPath() != "")
		path = _location->getUploadedPath();
	else
		path = getPath(request, config);
	size_t boundaryPos = request.getContentType().find("boundary=");
	if (boundaryPos != std::string::npos)
	{
		std::string boundary = "--" + request.getContentType().substr(boundaryPos + 9, request.getContentType().size());
		std::cout << "Boundary: " << boundary << std::endl;
		std::string body = request.getBody().substr(boundary.size() + 2, request.getBody().size());
		std::string formField;
		while (body.find(boundary)!=std::string::npos)
		{
			formField = body.substr(0, body.find(boundary) - 1);
			body = body.substr(body.find(boundary) + boundary.size() + 2, body.size());
			std::cout << "### " << formField << std::endl;
			uploadFile(path, formField);
		}
	}
	if (_code == "")
		getErrorPage(config, "409");
}

void Response::postMethod(Request request, Config config)//Dont know what response send if no files send only fields
{
	(void)config;
	std::string file = request.getFile();

	std::cout << "POST 2" << std::endl;
	if (_location and _location->getCgiExt()!= "" and getExtension(file) == _location->getCgiExt())//cgi extension config file
	{
		std::string path;
		if (_location->getCgiPath() != "")
			path = _location->getCgiPath();
		else
			path = getPath(request, config);
		CgiHandler cgi(request, config, path);
		if (cgi.getError().empty())
			_cgiResponse = cgi.getResponse();
		else
			getErrorPage(config, cgi.getError());
	}
	else if (_location and _location->getAllowUploads())
	{
		
		std::cout << "POST 3" << std::endl;
		staticPost(request, config);
	}
	else
		getErrorPage(config, "403");
}

void Response::deleteMethod(Request request, Config config)
{
	std::string path = getPath(request, config);
	if (request.getFile() == "")
		getCode("noidea");
	path += request.getFile();
	std::remove(path.c_str());
	getCode("204");
	//check for errors to send correct error page like not allowed
}
