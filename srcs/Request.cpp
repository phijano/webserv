/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phijano- <phijano-@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 10:19:26 by phijano-          #+#    #+#             */
/*   Updated: 2024/02/29 12:41:46 by phijano-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

Request::Request()
{
}

Request::Request(std::string request)
{
	_error = false;
	parseRequest(request);
}

Request::Request(const Request& other)
{
	*this = other;
}

Request& Request::operator=(const Request& other)
{
	_method = other._method;
	_path = other._path;
	_file = other._file;
	_query = other._query;
	_protocol = other._protocol;
	_host = other._host;
	_contentType = other._contentType;
	_contentLength = other._contentLength;
	_cgiHeaderParams = other._cgiHeaderParams;
	_body = other._body;
	_error = other._error;

	return *this;
}

Request::~Request()
{
}

std::string Request::getMethod() const
{
	return _method;
}

std::string Request::getPath() const
{
	return _path;
}

std::string Request::getFile() const
{
	return _file;
}

std::string Request::getQuery() const
{
	return _query;
}

std::string Request::getProtocol() const
{
	return _protocol;
}

std::string Request::getHost() const
{
	return _host;
}

std::string Request::getContentType() const
{
	return _contentType;
}

std::string Request::getContentLength() const
{
	return _contentLength;
}

std::map<std::string, std::string> Request::getCgiHeaderParams() const
{
	return _cgiHeaderParams;
}
std::string Request::getBody() const
{
	return _body;
}

bool Request::getError() const
{
	return _error;
}

void Request::parseUrl(std::string url)
{
	size_t paramPos;

	paramPos = url.find("?");
	if (paramPos != std::string::npos)	
	{
		_path = url.substr(0, paramPos);
		_query = url.substr(paramPos + 1, url.size());
		std::cout << "Query: " << _query << std::endl;
	} 
	else
		_path = url;
	paramPos = _path.find_last_of("/");
	if (paramPos != std::string::npos)
	{
		_file = _path.substr( _path.find_last_of("/") + 1, _path.size());
		_path = _path.substr(0, _path.find_last_of("/") + 1);
	}
}

void Request::parseFirstLine(std::string line)
{
	std::stringstream ssLine(line);
	std::string url;

	ssLine >> _method;
	ssLine >> url;
	parseUrl(url);
	ssLine >> _protocol;

}

void Request::parseHost(std::string hostLine)
{
	std::stringstream ssLine(hostLine);
	std::string word;

	ssLine >> word;
	ssLine >> _host;
}

void Request::parseHeader(std::string header)
{
	std::stringstream ss(header);
	std::string line;
	std::string word;
	size_t paramSepPos;

	std::cout << "HEADER:\n" << header << "<-" << std::endl;
	getline(ss, line);
	parseFirstLine(line);
	getline(ss, line);
	parseHost(line);
	while (getline(ss, line))
	{
		paramSepPos = line.find(":");
		if (paramSepPos!=std::string::npos)
		{
			word = line.substr(0, paramSepPos);
			if (word == "Content-Type")
				_contentType = line.substr(paramSepPos + 2, line.size() - paramSepPos - 3);
			else if (word == "Content-Length")
				_contentLength = line.substr(paramSepPos + 2, line.size() - paramSepPos - 3);
			else
				_cgiHeaderParams["HTTP_" + word] = line.substr(paramSepPos + 2, line.size() - paramSepPos - 3);
		}
	}
}

void Request::checkRequest()
{
	if (_method == "" or _path == "" or _path[0] != '/' or _protocol == "")
		_error = true;
}

void Request::parseRequest(std::string request)
{
	std::stringstream ss(request);
	std::string line;
	std::string word;	
	
	size_t headerEnd = ss.str().find("\r\n\r\n");
	if (headerEnd != std::string::npos)
	{
		parseHeader(ss.str().substr(0, headerEnd + 2));
		_body = ss.str().substr(headerEnd + 4, ss.str().size());
		std::cout << "BODY:\n" << _body << "<-" << std::endl;
	}
	checkRequest();
}

