#include "Config.hpp"

Config::Config(){}
Config::~Config(){}

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