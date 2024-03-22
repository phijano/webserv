/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vnaslund <vnaslund@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 14:27:05 by vnaslund          #+#    #+#             */
/*   Updated: 2024/03/21 19:34:15 by vnaslund         ###   ########.fr       */
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

std::vector<Config>&	ConfigParser::getServers(void)
{
	return (_servers);
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

	for (size_t i = 0; i < _nbr_servers; i++)
	{
		Config	server;
		parseServer(_server_contents[i], server);
		_servers.push_back(server);
	}
}

void	ConfigParser::parseServer(std::string& content, Config& server)
{
	bool						serverAutoIndex = false;
	std::vector<std::string>	tokens;
	
	tokens = splitContent(content);

	for (size_t i = 0; i < tokens.size(); i++)
	{
		if (tokens[i] == "host")
		{
			i++;
			server.setHost(tokens[i].substr(0, tokens[i].length() - 1));
		}
		else if (tokens[i] == "root")
		{
			i++;
			server.setRoot(tokens[i].substr(0, tokens[i].length() - 1));
		}
		else if (tokens[i] == "index")
		{
			i++;
			server.setIndex(tokens[i].substr(0, tokens[i].length() - 1));
		}
		else if (tokens[i] == "upload_dir")
		{
			i++;
			server.setUploadDir(tokens[i].substr(0, tokens[i].length() - 1));
		}
		else if (tokens[i] == "server_name")
		{
			while (++i < tokens.size())
			{
				if (tokens[i][tokens[i].length() - 1] == ';')
				{
					server.addServerName(tokens[i].substr(0, tokens[i].length() - 1));
					break ;
				}
				else
					server.addServerName(tokens[i]);
			}
		}
		else if (tokens[i] == "listen")
		{
			std::string::size_type delim = tokens[++i].find(':');
			if (delim == std::string::npos)
				server.setPort(std::stoi(tokens[i]));
			else
			{
				server.setHost(tokens[i].substr(0, delim));
				server.setPort(std::stoi(tokens[i].substr(delim + 1)));
			}
		}
		else if (tokens[i] == "error_page")
		{
			int	error_code = std::stoi(tokens[++i]);
			i++;
			server.addErrorPage(error_code, tokens[i].substr(0, tokens[i].length() - 1));
		}
		else if (tokens[i] == "client_max_body_size")
		{
			int	size = std::stoi(tokens[++i]);
			server.setBodySize(size);
		}
		else if (tokens[i] == "autoindex" || tokens[i] == "autoindex:")
		{
			i++;
			if (tokens[i] == "on;" || tokens[i] == "yes;")
				serverAutoIndex = true;
				
		}
		else if (tokens[i] == "location")
		{
			Location	location;
			
			if (serverAutoIndex)
				location.setAutoIndex(true);
			location.setPath(tokens[++i]);
			location.setRoot(server.getRoot());
			while (tokens[++i] != "}")
			{
				if (tokens[i] == "root")
				{
					i++;
					location.setRoot(tokens[i].substr(0, tokens[i].length() - 1));
				}
				else if (tokens[i] == "index")
				{
					i++;
					location.setIndex(tokens[i].substr(0, tokens[i].length() - 1));
				}
				else if (tokens[i] == "upload_path")
				{
					i++;
					location.setUploadPath(tokens[i].substr(0, tokens[i].length() - 1));
				}
				else if (tokens[i] == "cgi_ext")
				{
					i++;
					location.setCgiExt(tokens[i].substr(0, tokens[i].length() - 1));
				}
				else if (tokens[i] == "cgi_path")
				{
					i++;
					location.setCgiPath(tokens[i].substr(0, tokens[i].length() - 1));
				}
				else if (tokens[i] == "allow_uploads:" || tokens[i] == "allow_uploads")
				{
					i++;
					if (tokens[i] == "on;" || tokens[i] == "yes;")
						location.setAllowUploads(true);
				}
				else if (tokens[i] == "autoindex" || tokens[i] == "autoindex:")
				{
					i++;
					if (tokens[i] == "on;" || tokens[i] == "yes;")
						location.setAutoIndex(true);
				}
				else if (tokens[i] == "allow_methods" || tokens[i] == "allow_methods:")
				{
					while (++i < tokens.size())
					{
						if (tokens[i][tokens[i].length() - 1] == ';')
						{
							location.addAllowedMethod(tokens[i].substr(0, tokens[i].length() - 1));
							break ;
						}
						else
							location.addAllowedMethod(tokens[i]);
					}
				}
			}
			server.addLocation(location);
		}
	}
}

std::vector<std::string>	ConfigParser::splitContent(const std::string& content)
{
	std::vector<std::string>	tokens;
	const std::string			separators = " \n\t";
	size_t						start = 0;
	size_t						end;

	while ((end = content.find_first_of(separators, start)) != std::string::npos)
	{
		if (end > start)
			tokens.push_back(content.substr(start, end - start));
		start = content.find_first_not_of(separators, end);
		if (start == std::string::npos)
			break ;
	}

	if (start < content.length())
		tokens.push_back(content.substr(start));

	return (tokens);
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
	size_t	i = start;
    bool	foundServer = false;

	while (i < content.length())
	{
		if (content.compare(i, 6, "server") == 0)
		{
			foundServer = true;
			i += 6;
			break ;
		}
		else if (!isspace(content[i]))
			throw ConfigFileException("Invalid character found outside of server scope");
		i++;
	}
	if (!foundServer)
		return (start);
	while (i < content.length() && isspace(content[i]))
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