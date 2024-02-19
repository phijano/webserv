/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiHandler.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phijano- <phijano-@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 12:44:14 by phijano-          #+#    #+#             */
/*   Updated: 2024/02/19 13:26:07 by phijano-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CgiHandler.hpp"

CgiHandler::CgiHandler()
{
}

CgiHandler::CgiHandler(Request request)
{
	(void)request;
	sendToCgi();
}

CgiHandler::CgiHandler(const CgiHandler& other)
{
	*this = other;
}

CgiHandler& CgiHandler::operator=(const CgiHandler& other)
{

	(void)other;
	return *this;
}

CgiHandler::~CgiHandler()
{
}

std::string CgiHandler::getResponse() const
{
	return _response;
}

std::string CgiHandler::getError() const
{
	return _error;
}

void CgiHandler::execCgi(int *fd, std::string path, std::string file, char **env)
{
	char *command[] = {(char *)file.c_str(), NULL};
	int error;

	(void) env;
	dup2(fd[1], STDOUT_FILENO);
	close(fd[1]);
	close(fd[0]);
	path = "." + path;
	error = execve(path.c_str(), command, env);
	std::cout << "Child: path: " << path.c_str() << " cgi_file: " << command[0] << std::endl;
	std::cout << "Error: " << error <<  " child exec fail" << std::endl;
	exit(1);
}

void CgiHandler::sendToCgi(void)
{
	std::cout << "CGI" << std::endl;

	pid_t pid;
	int fd[2];
	char buffer[30720];
	std::string file = "test.cgi";
	char *env[] = {(char*)"REQUEST_METHOD=GET", (char*)"SERVER_PROTOCOL=HTTP/1.1", NULL};

	pipe(fd);
	pid = fork();
	if (pid == -1)
		_error = "505";
	else if (pid == 0)
		execCgi(fd, "/testweb/test.cgi", file, env);
	else
	{
		int status;
		std::cout << "READ" << std::endl;
		close(fd[1]);
		if (read(fd[0], buffer, 30720) > 0)
		{
			std::cout << "Father read: " << buffer << std::endl;
		}
		_response = buffer;
		std::cout << "CGI: *** " << _response << "<-" << std::endl;
		close(fd[0]);
		waitpid(pid, &status, 0);
	}
	std::cout << "***" << std::endl;
}
