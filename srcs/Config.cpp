#include "Config.hpp"

Config::Config():
	host("localhost"),
    port(80),
    bodySize("1MB"),
    root("/var/www"),
    index("index.html")
{}

Config::~Config(){}

// Getter Methods

int Config::getPort() {
    return this->port;
}

std::string Config::getRoot() {
    return this->root;
}

std::string Config::getIndex() {
    return this->index;
}

std::vector<std::string> Config::getServerNames() {
    return this->serverNames;
}

std::vector<ErrorPage> Config::getErrorPages() {
    return this->errors;
}

std::vector<Location> Config::getLocations() {
    return this->locations;
}

// Setter Methods
void Config::setHost(const std::string newHost) {
    host = newHost;
}

void Config::addServerName(std::string newServerName) {
    serverNames.push_back(newServerName);
}

void Config::setPort(int newPort) {
    port = newPort;
}

void Config::setBodySize(const std::string newSize) {
    bodySize = newSize;
}

void Config::setRoot(const std::string newRoot) {
    root = newRoot;
}

void Config::setIndex(const std::string newIndex) {
    index = newIndex;
}

void Config::addErrorPage(ErrorPage newError) {
    errors.push_back(newError);
}

void Config::addLocation(Location newLocation) {
    locations.push_back(newLocation);
}

std::ostream& operator<<(std::ostream& os, const Config& config)
{
    os << "Host: " << config.host << "\nPort: " << config.port
       << "\nRoot: " << config.root << "\nIndex: " << config.index
       << "\nServer Names: ";
    for (size_t i = 0; i < config.serverNames.size(); ++i) {
        os << config.serverNames[i] << " ";
    }
    os << "\nError Pages:\n";
    for (size_t i = 0; i < config.errors.size(); ++i) {
        os << config.errors[i] << "\n";
    }
    os << "Locations:\n";
    for (size_t i = 0; i < config.locations.size(); ++i) {
        os << config.locations[i] << "\n";
    }
    return os;
}