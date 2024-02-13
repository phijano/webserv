/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phijano- <phijano-@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 10:19:26 by phijano-          #+#    #+#             */
/*   Updated: 2024/02/13 11:58:40 by phijano-         ###   ########.fr       */
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
	_parameters = other._parameters;
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

std::vector<std::vector<std::string> > Request::getParameters() const
{
	return _parameters;
}

bool Request::getError() const
{
	return _error;
}

void Request::parseParameter(std::string formField)
{
	std::stringstream ss(formField);
	std::string line;
	std::string value;
	size_t startValue;
	size_t endValue;
	std::vector <std::string> param;

	getline(ss, line);
	startValue = line.find("\"");
	endValue = line.find("\"", startValue + 1);
	value = line.substr(startValue + 1, endValue - startValue - 1);
//	std::cout << "name " << value << std::endl;
	param.push_back(value);

	startValue = line.find("\"", endValue + 1);
	if (startValue != std::string::npos)
	{
		value = line.substr(startValue + 1, line.find("\"", startValue + 1) - startValue - 1);
//		std::cout << "filename " << value << std::endl;
		param.push_back(value);
	}
	getline(ss, line);

	if (line != "\r")
		getline(ss, line);
	getline(ss, value, '\r');
//	std::cout << "Content: " << value << "<-Content"<< std::endl;
	param.push_back(value);
	_parameters.push_back(param);
}

void Request::parseUrl(std::string url)//need to parse parameters
{
	size_t paramPos;

	paramPos = url.find("?");
	if (paramPos != std::string::npos)
	{
		_path = url.substr(0, paramPos);
	}
	else
		_path = url;
	_file = _path.substr( _path.find_last_of("/") + 1, _path.size());
	_path = _path.substr(0, _path.find_last_of("/") + 1);
}


void Request::parseRequest(std::string request)
{
	std::stringstream ss(request);
	std::string line;
	std::string word;

	if (getline(ss, line))
	{
		std::stringstream ssLine(line);
		ssLine >> word;
		if (word == "GET" or word == "POST" or word == "DELETE")
		{
			_method = word;
			std::cout << "Method: " << _method << std::endl;
			ssLine >> word;
			_path = "testweb" + word; //add root to path; this will be in response we need to check location for root
			if (_method == "GET")
				_path += "index.html"; //add index to path; this in response too
			std::cout << "Path: " << _path << std::endl;
			parseUrl(word);

			size_t boundaryPos = ss.str().find("boundary=");
			if (boundaryPos != std::string::npos)
			{
				std::string boundary;
				std::stringstream ssLine(ss.str().substr(boundaryPos + 9, ss.str().size()));
				getline(ssLine, boundary, '\r');
				boundary = "--" + boundary;
				std::cout << "Boundary: " << boundary << std::endl;
				std::string body =	ss.str().substr(ss.str().find(boundary) + boundary.size() + 2, ss.str().size());
				std::string formField;
				while (body.find(boundary)!=std::string::npos)
				{
					formField = body.substr(0, body.find(boundary) - 1);
					body = body.substr(body.find(boundary) + boundary.size() + 2, body.size());
					std::cout << "### " << formField << std::endl;
					parseParameter(formField);
				}
			}
			else
			{
				std::cout << "no body" << std::endl;
			}
		}
	}
}
