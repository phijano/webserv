/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phijano- <phijano-@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 10:57:54 by phijano-          #+#    #+#             */
/*   Updated: 2024/02/13 12:24:12 by phijano-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

Response::Response()
{
}

Response::Response(Request request)
{
	_protocol = "HTTP/1.1";
	if (request.getError())
		getErrorPage("400");
	else if (request.getMethod() == "GET")
	{
		std::cout << "GET" << std::endl;
		getMethod("testweb" + request.getPath(), request.getFile());//change testweb for root
	}
	else if (request.getMethod() == "POST")
	{
		std::cout << "POST" << std::endl;
		postMethod("testweb" + request.getPath(), request.getParameters());//idem
	}
	else if (request.getMethod() == "DELETE")
	{
		std::cout << "DELETE" << std::endl;
		deleteMethod("testweb" + request.getPath(), request.getFile());//idem
	}
	else
		getErrorPage("501");
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
}

std::string Response::getResponse() const
{
	std::stringstream response;

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

void Response::getMime(std::string file)//no idea how many to put here
{
	std::string ext;
	ext = file.substr(file.find_last_of("."), file.size());

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


void Response::getErrorPage(std::string error)
{
	std::stringstream resource;

	_code = "500 Internal Server Error";
	_mime = "text/html";
	_body = "<!DOCTYPE html><html lang=\"en\"><body><h1> 500 Internal Server Error </h1><p> Whooops! </p></body></html>";
	std::ifstream file("errorpages/" + error + ".html"); //look if there was other error pages provided in config
	if (file.is_open())
	{
		getCode(error);
		resource << file.rdbuf();
		_body = resource.str();
	}
}

void Response::getMethod(std::string path, std::string file)
{
	std::stringstream resource;
	std::stringstream response;

	if (file == "")
		file = "index.html";//config index
	std::ifstream fileStream(path + file);
	if (fileStream.is_open())
	{
		resource << fileStream.rdbuf();
		getCode("200");
		getMime(file);
		_body = resource.str();
	}
	else
		getErrorPage("404");
}

void Response::postMethod(std::string path, std::vector<std::vector<std::string> > parameters)//Dont know what response send if no files send only string fields
{
	bool isAnyCreated = false;

	for (std::vector<std::vector<std::string> >::iterator it = parameters.begin(); it != parameters.end(); it++)
	{
		if ((*it).size() == 3)
		{
			std::ifstream file(path + (*it)[1]); // path + filename
			if (!file.good())
			{
				std::ofstream newFile(path + (*it)[1]);
				newFile << (*it)[2]; //file content
				newFile.close();
				isAnyCreated = true;
				getCode("201");
			}
		}
	}
	if (!isAnyCreated)
		getErrorPage("409");
}

void Response::deleteMethod(std::string path, std::string file)
{
	if (file == "")
		getCode("noidea");
	path = path + file;
	std::remove(path.c_str());
	getCode("204");
	//check for errors to send correct error page like not allowed
}
