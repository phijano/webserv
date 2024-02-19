/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vnaslund <vnaslund@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 14:27:05 by vnaslund          #+#    #+#             */
/*   Updated: 2024/02/19 19:43:59 by vnaslund         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigParser.hpp"

ConfigParser::ConfigParser()
{
	_nbr_servers = 0;
}

ConfigParser::~ConfigParser()
{
	
}

void	ConfigParser::parseFile(const std::string& fileName)
{
	if (!isValidConfigFile(fileName))
        throw ConfigFileException("Invalid or nonexistent file");
	
	std::string			content;
	std::ifstream 		file(fileName);
	std::stringstream	buffer;
	
	buffer << file.rdbuf();
	content = buffer.str();
	file.close();
	removeWhitespace(content);
	splitServers(content);
}

void	ConfigParser::splitServers(std::string& content)
{
	size_t	start = 0;
	size_t	end = 1;

	if (content.find("server", 0) == std::string::npos)
		throw ConfigFileException("no server found in .conf file");
	
	while (start != end && start < content.length())
	{
		start = findStartOfServer(start, content);
		end = findEndOfServer(start, content);
		if (start == end)
			throw ConfigFileException("parsing error");
		_server_contents.push_back(content.substr(start, end - start + 1));
		_nbr_servers++;
		start = end + 1;
	}
}

size_t	ConfigParser::findStartOfServer(size_t start, std::string& content)
{
	size_t i;

	for (i = start; content[i]; i++)
	{
		if (content[i] == 's')
			break ;
		else if (!isspace(content[i]))
			throw ConfigFileException("Invalid character found outside of server scope");
	}
	if (!content[i])
		return (start);
	if (content.compare(i, 6, "server") != 0)
		throw ConfigFileException("Wrong character found outside of server scope");
	i += 6;
	while (content[i] && isspace(content[i]))
		i++;
	if (content[i] == '{')
		return (i);
	else
		throw ConfigFileException("Invalid server configuration");
}

size_t	ConfigParser::findEndOfServer(size_t start, std::string& content)
{
	size_t	i;
	size_t	open_brackets = 1;
	
	for (i = start + 1; content[i]; i++)
	{
		if (content[i] == '{')
			open_brackets++;
		if (content[i] == '}')
		{
			open_brackets--;
			if (open_brackets == 0)
				return (i);
		}
	}
	return (start);
}

bool	ConfigParser::isValidConfigFile(const std::string& fileName)
{
	bool		hasExtension;
	bool		canBeOpened;
	std::string	extension = ".conf";

	if (fileName.size() >= extension.size() && 
		fileName.compare(fileName.size() - extension.size(), extension.size(), extension) == 0)
		hasExtension = true;
	else
		hasExtension = false;
	
	std::ifstream	file(fileName.c_str());
	canBeOpened = file.is_open();
	file.close();

	return (hasExtension && canBeOpened);
}

void	ConfigParser::removeWhitespace(std::string& content)
{
	size_t	i = 0;

	while (content[i] && isspace(content[i]))
		i++;
	content = content.substr(i);
	i = content.length() - 1;
	while (i > 0 && isspace(content[i]))
		i--;
	content = content.substr(0, i + 1);
}

ConfigParser::ConfigFileException::ConfigFileException(const std::string& msg) : message("Config File Error: " + msg)
{

}

const char* ConfigParser::ConfigFileException::what() const throw()
{
    return (message.c_str());
}