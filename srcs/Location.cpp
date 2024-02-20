#include "Location.hpp"

Location::Location(){}
Location::~Location(){}

std::string Location::getRoute() {
    return this->route;
}

std::string Location::getRoot() {
    return this->root;
}

std::string Location::getIndex() {
    return this->index;
}

std::string Location::getCgiExt() {
    return this->cgiExt;
}

std::string* Location::getAllowMethods() {
    return this->allowMethods;
}

std::string Location::getUploadedPath() {
    return this->uploadedPath;
}

bool Location::getAutoIndex() {
    return this->autoIndex;
}

bool Location::getAllowUploads() {
    return this->allowUploads;
}