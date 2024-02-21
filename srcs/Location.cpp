#include "Location.hpp"

Location::Location():
    route(""),
    root(""),
    index(""),
    cgiExt(""),
    allowMethods(nullptr),
    uploadedPath(""),
    autoIndex(false),
    allowUploads(false)
{}

Location::~Location(){}

// Getter Methods
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
// Setter Methods
void Location::setRoute(const std::string newRoute) {
    route = newRoute;
}

void Location::setRoot(const std::string newRoot) {
    root = newRoot;
}

void Location::setIndex(const std::string newIndex) {
    index = newIndex;
}

void Location::setCgiExt(const std::string newCgiExt) {
    cgiExt = newCgiExt;
}

void Location::setAllowMethods(std::string* newAllowMethods) {
    allowMethods = newAllowMethods;
}

void Location::setUploadedPath(const std::string newUploadedPath) {
    uploadedPath = newUploadedPath;
}

void Location::setAutoIndex(bool newAutoIndex) {
    autoIndex = newAutoIndex;
}

void Location::setAllowUploads(bool newAllowUploads) {
    allowUploads = newAllowUploads;
}

std::ostream& operator<<(std::ostream& os, const Location& location)
{
    os << "Route: " << location.route << std::endl
       << "Root: " << location.root << std::endl
       << "Index: " << location.index << std::endl
       << "CGI Extension: " << location.cgiExt << std::endl
       << "Uploaded Path: " << location.uploadedPath << std::endl
       << "Auto Index: " << (location.autoIndex ? "yes" : "no") << std::endl
       << "Allow Uploads: " << (location.allowUploads ? "yes" : "no");

    // For allowMethods, assuming it's an array of strings terminated by a nullptr or similar sentinel
    if (location.allowMethods) {
        os << std::endl << "Allowed Methods: ";
        for (int i = 0; location.allowMethods[i] != ""; ++i) { // Replace condition based on actual termination condition
            os << location.allowMethods[i];
            if (location.allowMethods[i + 1] != "") os << ", "; // Check next is not terminator for formatting
        }
    }
    
    return os;
}