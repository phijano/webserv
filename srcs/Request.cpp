/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phijano- <phijano-@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 10:19:26 by phijano-          #+#    #+#             */
/*   Updated: 2024/02/27 13:33:39 by phijano-         ###   ########.fr       */
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
	if (paramPos != std::string::npos)//queryParse
	{
		_path = url.substr(0, paramPos);
		_query = url.substr(paramPos + 1, url.size());
		std::cout << "Query: " << _query << std::endl;
	} 
	else
		_path = url;
	_file = _path.substr( _path.find_last_of("/") + 1, _path.size());
	_path = _path.substr(0, _path.find_last_of("/") + 1);
}

void Request::parseHeader(std::string header)
{
	std::stringstream ss(header);
	std::string line;
	std::string word;
	size_t paramSepPos;

	std::cout << "HEADER:\n" << header << "<-" << std::endl;
	getline(ss, line);
	std::stringstream ssLine(line);
	ssLine >> _method;
	std::cout << "Method: " << _method << std::endl;
	ssLine >> word;
	ssLine >> _protocol;
	parseUrl(word);
	std::cout << "Path: " << _path << _file << std::endl;
	getline(ss, line);
	ssLine.str(line);
	ssLine.clear();
	ssLine >> word;
	ssLine >> _host;
	std::cout << "Host: " << _host << std::endl;
	while (getline(ss, line))
	{
		paramSepPos = line.find(":");
		if (paramSepPos!=std::string::npos)
		{
			word = line.substr(0, paramSepPos);
			if (word == "Content-Type")
			{
				_contentType = line.substr(paramSepPos + 2, line.size() - paramSepPos - 3);
				std::cout << "CType: " << _contentType << std::endl;
				continue;
			}
			else if (word == "Content-Length")
			{
				_contentLength = line.substr(paramSepPos + 2, line.size() - paramSepPos - 3);
				std::cout << "CLength: " << _contentLength << std::endl;
				continue;
			}
			else
			{
				_cgiHeaderParams["HTTP_" + word] = line.substr(paramSepPos + 2, line.size() - paramSepPos - 3);
				std::cout << word << " : " << _cgiHeaderParams["HTTP_" + word] << std::endl;
			}
		}
	}
}

void Request::parseRequest(std::string request)
{
	std::stringstream ss(request);
	std::string line;
	std::string word;

	size_t headerEnd = ss.str().find("\r\n\r\n");
	if (headerEnd == std::string::npos)
	{
		std::cout << "header: " << request << std::endl;
		_error = true;
		return;
	}
	parseHeader(ss.str().substr(0, headerEnd + 2));
	_body = ss.str().substr(headerEnd + 4, ss.str().size());
}

