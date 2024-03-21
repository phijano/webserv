/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vnaslund <vnaslund@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 17:02:38 by vnaslund          #+#    #+#             */
/*   Updated: 2024/03/21 17:03:07 by vnaslund         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"

Location::Location():
    path(""),
    root(""),
    index(""),
    cgiExt(""),
    uploadedPath(""),
    autoIndex(false),
    allowUploads(false)
{}

Location::~Location(){}

// Getter Methods
std::string Location::getPath() {
    return this->path;
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

std::string Location::getCgiPath() {
    return this->cgiPath;
}

std::vector<std::string>	Location::getAllowedMethods() {
    return this->allowedMethods;
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
void Location::setPath(const std::string newPath) {
    path = newPath;
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

void Location::setCgiPath(const std::string newCgiPath) {
    cgiPath = newCgiPath;
}

void Location::addAllowedMethod(std::string newMethod) {
    allowedMethods.push_back(newMethod);
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

std::ostream& operator<<(std::ostream& os, Location& location) 
{
    os << "Location Path: " << location.getPath() << std::endl
       << "Root: " << location.getRoot() << std::endl
       << "Index: " << location.getIndex() << std::endl
       << "CGI Extension: " << location.getCgiExt() << std::endl
       << "CGI Path: " << location.getCgiPath() << std::endl
       << "Uploaded Path: " << location.getUploadedPath() << std::endl
       << "Auto Index: " << (location.getAutoIndex() ? "yes" : "no") << std::endl
       << "Allow Uploads: " << (location.getAllowUploads() ? "yes" : "no") << std::endl
       << "Allowed Methods: ";
    const std::vector<std::string>& methods = location.getAllowedMethods();
    for (size_t i = 0; i < methods.size(); ++i) {
        os << methods[i];
        if (i < methods.size() - 1) os << ", ";
    }
    os << std::endl;
    return os;
}
