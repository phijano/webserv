#include "Config.hpp"

Config::Config():
	host("localhost"),
    serverNames(nullptr),
    port(80),
    bodySize("1MB"),
    root("/var/www"),
    index("index.html"),
    errors(nullptr),
    locations(nullptr)
{
	
}

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

std::string* Config::getServerNames() {
    return this->serverNames;
}

ErrorPage* Config::getErrorPages() {
    return this->errors;
}

Location* Config::getLocations() {
    return this->locations;
}

// Setter Methods
void Config::setHost(const std::string newHost) {
    host = newHost;
}

void Config::setServerNames(std::string* newServerNames) {
    serverNames = newServerNames;
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

void Config::setErrors(ErrorPage* newErrors) {
    errors = newErrors;
}

void Config::setLocations(Location* newLocations) {
    locations = newLocations;
}