/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vnaslund <vnaslund@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 17:02:29 by vnaslund          #+#    #+#             */
/*   Updated: 2024/04/04 17:22:48 by vnaslund         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"

Config::Config():
	host("localhost"),
    port(80),
    bodySize(1),
    root("web"),
    index(""),
	uploadDir("../")
{}

Config::~Config(){}

// Getter Methods

std::string Config::getHost() const
{
    return this->host;
}

int Config::getPort() const
{
    return this->port;
}

int	Config::getBodySize() const
{
	return this->bodySize;
}

std::string Config::getRoot() const
{
    return this->root;
}

std::string Config::getIndex() const
{
    return this->index;
}

std::string Config::getUploadDir() const
{
    return this->uploadDir;
}

std::vector<std::string> Config::getServerNames() const
{
    return this->serverNames;
}

std::map<int, std::string> Config::getErrorPages() const
{
    return this->errors;
}

std::vector<Location> Config::getLocations() const
{
    return this->locations;
}

// Setter Methods
void Config::setHost(const std::string newHost)
{
    host = newHost;
}

void Config::addServerName(std::string newServerName)
{
    serverNames.push_back(newServerName);
}

void Config::setPort(int newPort)
{
    port = newPort;
}

void Config::setBodySize(int newSize)
{
    bodySize = newSize;
}

void Config::setRoot(const std::string newRoot)
{
    root = newRoot;
}

void Config::setIndex(const std::string newIndex)
{
    index = newIndex;
}

void Config::setUploadDir(const std::string newDir)
{
    uploadDir = newDir;
}

void Config::addErrorPage(int code, std::string path)
{
    errors[code] = path;
}

void Config::addLocation(Location newLocation)
{
    locations.push_back(newLocation);
}

std::ostream& operator<<(std::ostream& os, Config& config)
{
    os << "Host: " << config.getHost() << "\nPort: " << config.getPort()
       << "\nRoot: " << config.getRoot() << "\nIndex: " << config.getIndex()
	   << "\nUpload dir: " << config.getUploadDir()
       << "\nBody size: " << config.getBodySize() << "\nServer Names: ";
    std::vector<std::string> serverNames = config.getServerNames();
    for (size_t i = 0; i < serverNames.size(); ++i) {
        os << serverNames[i] << " ";
    }
    os << "\nError Pages:\n";
    const std::map<int, std::string>& errors = config.getErrorPages();
    for (std::map<int, std::string>::const_iterator it = errors.begin(); it != errors.end(); ++it) {
        os << "Code " << it->first << ": " << it->second << "\n";
    }
    os << "\nLocations:\n";
    std::vector<Location> locations = config.getLocations();
    for (size_t i = 0; i < locations.size(); ++i) {
        os << locations[i] << "\n";
    }
    return os;
}