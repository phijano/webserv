/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vnaslund <vnaslund@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 14:27:05 by vnaslund          #+#    #+#             */
/*   Updated: 2024/02/19 15:04:23 by vnaslund         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigParser.hpp"

ConfigParser::ConfigParser()
{
	
}

ConfigParser::~ConfigParser()
{
	
}

void	ConfigParser::parseFile(const std::string& fileName)
{
	if (!isValidConfigFile(fileName))
        throw ConfigFileException("Invalid or nonexistent file");
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

ConfigParser::ConfigFileException::ConfigFileException(const std::string& msg) : message("Config File Error: " + msg)
{

}

const char* ConfigParser::ConfigFileException::what() const throw()
{
    return message.c_str();
}