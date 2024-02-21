/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phijano- <phijano-@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 10:19:26 by phijano-          #+#    #+#             */
/*   Updated: 2024/02/21 14:46:39 by phijano-         ###   ########.fr       */
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
	_contentType = other._contentType;
	_contentLength = other._contentLength;
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

std::string Request::getContentType() const
{
	return _contentType;
}

std::string Request::getContentLength() const
{
	return _contentLength;
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

	std::cout << "HEADER:\n" << header << "<-" << std::endl;
	getline(ss, line);
	std::stringstream ssLine(line);
	ssLine >> word;
	_method = word;
	std::cout << "Method: " << _method << std::endl;
	ssLine >> word;
	parseUrl(word);
	std::cout << "Path: " << _path << _file << std::endl;
	size_t contentPos = ss.str().find("Content-Type: ");
	if (contentPos != std::string::npos)
	{
		std::stringstream ssLine(ss.str().substr(contentPos + 14, ss.str().size()));
		getline(ssLine, _contentType, '\r');
		std::cout << "Content-Type: " << _contentType << std::endl;
		contentPos = ss.str().find("Content-Length: ");
		if (contentPos != std::string::npos)
		{
			std::stringstream ssLine(ss.str().substr(contentPos + 16, ss.str().size()));
			getline(ssLine, _contentLength, '\r');
			std::cout << "Content-Length: " << _contentLength << std::endl;
		}
	}
}

void Request::parseRequest(std::string request)
{
	std::stringstream ss(request);
	std::string line;
	std::string word;

	size_t headerEnd = ss.str().find("\r\n\r\n");
	parseHeader(ss.str().substr(0, headerEnd + 2));
	_body = ss.str().substr(headerEnd + 4, ss.str().size());
}

