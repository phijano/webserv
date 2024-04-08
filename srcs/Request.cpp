/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vnaslund <vnaslund@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 10:19:26 by phijano-          #+#    #+#             */
/*   Updated: 2024/04/08 19:09:10 by vnaslund         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

Request::Request()
{
}

Request::Request(const std::string& request)
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
	_clientIp = other.getClientIp();
	_method = other.getMethod();
	_path = other.getPath();
	_file = other.getFile();
	_query = other.getQuery();
	_pathInfo = other.getPathInfo();
	_protocol = other.getProtocol();
	_host = other.getHost();
	_contentType = other.getContentType();
	_contentLength = other.getContentLength();
	_cgiHeaderParams = other.getCgiHeaderParams();
	_body = other.getBody();
	_error = other.getError();

	return *this;
}

Request::~Request()
{
}

void Request::setClientIp(unsigned long clientIp)
{
	char *p;
	std::stringstream ss;

	p = reinterpret_cast<char *>(&clientIp);
	ss	<<  static_cast<int>(p[0]) << "." << static_cast<int>(p[1]) << "." << static_cast<int>(p[2]) << "." <<  static_cast<int>(p[3]); 
	_clientIp = ss.str();
}

std::string Request::getClientIp() const
{
	return _clientIp;
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

std::string Request::getPathInfo() const
{
	return _pathInfo;
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

void Request::parseUrl(const std::string& url)
{
	size_t paramPos;

	paramPos = url.find("?");
	if (paramPos != std::string::npos)	
	{
		_path = url.substr(0, paramPos);
		_query = url.substr(paramPos + 1, url.size() - paramPos -1);
	} 
	else
		_path = url;
	paramPos = _path.find(".");
	if (paramPos != std::string::npos)
	{
		paramPos = _path.find("/", paramPos);
		if (paramPos != std::string::npos)
		{
			_pathInfo = _path.substr(paramPos, _path.size() - paramPos);
			_path = _path.substr(0, paramPos);
		}
		paramPos = _path.find_last_of("/");
		if (paramPos != std::string::npos)
		{
			_file = _path.substr(paramPos + 1, _path.size() - paramPos - 1);
			_path = _path.substr(0, paramPos + 1);
		}
	}
}

void Request::parseFirstLine(const std::string& line)
{
	std::stringstream ssLine(line);
	std::string url;

	ssLine >> _method;
	ssLine >> url;
	parseUrl(url);
	ssLine >> _protocol;

}

void Request::parseHost(const std::string& hostLine)
{
	std::stringstream ssLine(hostLine);
	std::string word;

	ssLine >> word;
	ssLine >> _host;
}

void Request::parseHeader(const std::string& header)
{
	std::stringstream ss(header);
	std::string line;
	std::string word;
	size_t paramSepPos;

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
	if (_method.empty() || _path.empty() || _path[0] != '/' || _protocol.empty())
		_error = true;
	else if (_method != "GET" and _method != "HEAD" and _method != "POST" and _method != "PUT" and _method != "DELETE" and _method != "CONNECT" and _method != "OPTIONS" and _method != "TRACE")
		_error = true;
	else if (_method == "POST" and (getContentType().empty() or getContentLength().empty()))
		_error = true;
}

void Request::parseRequest(const std::string& request)
{
	std::stringstream ss(request);
	std::string line;
	std::string word;	
	
	size_t headerEnd = ss.str().find("\r\n\r\n");
	if (headerEnd != std::string::npos)
	{
		parseHeader(ss.str().substr(0, headerEnd + 2));
		_body = ss.str().substr(headerEnd + 4, ss.str().size());
	}
	checkRequest();
}

std::ostream& operator<<(std::ostream& os, Request& request)
{
    os << "Client IP: " << request._clientIp << std::endl;
    os << "Method: " << request._method << std::endl;
    os << "Path: " << request._path << std::endl;
    os << "File: " << request._file << std::endl;
    os << "Query: " << request._query << std::endl;
    os << "Path Info: " << request._pathInfo << std::endl;
    os << "Protocol: " << request._protocol << std::endl;
    os << "Host: " << request._host << std::endl;
    os << "Content Type: " << request._contentType << std::endl;
    os << "Content Length: " << request._contentLength << std::endl;
    os << "Error: " << (request._error ? "true" : "false") << std::endl;

    os << "CGI Header Params:" << std::endl;
    std::map<std::string, std::string>::const_iterator it;
    for (it = request._cgiHeaderParams.begin(); it != request._cgiHeaderParams.end(); ++it) {
        os << it->first << ": " << it->second << std::endl;
    }

    os << "Body:" << std::endl;
    os << request._body << std::endl;

    return os;
}
