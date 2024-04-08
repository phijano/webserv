/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiHandler.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phijano- <phijano-@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 12:44:14 by phijano-          #+#    #+#             */
/*   Updated: 2024/04/08 13:18:51 by phijano-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CgiHandler.hpp"

CgiHandler::CgiHandler()
{
}

CgiHandler::CgiHandler(const Request& request, const Config& config, const std::string& path)
{
	_path = path;
	sendToCgi(request, config);
}

CgiHandler::CgiHandler(const CgiHandler& other)
{
	*this = other;
}

CgiHandler& CgiHandler::operator=(const CgiHandler& other)
{

	_path = other._path;
	_response = other._response;
	_error = other._error;
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

void CgiHandler::freeEnv()
{
	int param = -1;

	while(_env[++param])
		free(_env[param]);
	free (_env);
}

char* CgiHandler::setEnvParam(const std::string& param)
{
	char *temp = new char[param.size() + 1];
	strcpy (temp, param.c_str());
	return temp;
}

std::string CgiHandler::intToString(const int& number)
{
	std::stringstream ss;
	ss << number;
	std::string str = ss.str();
	return str;
}

std::string CgiHandler::toUppercase(std::string str)
{
	transform(str.begin(), str.end(), str.begin(), ::toupper);
	return str;
}

void CgiHandler::setCgiEnv(const Request& request, const Config& config)
{
	std::map<std::string, std::string> params = request.getCgiHeaderParams();
	int varsNumber = 10;
	
	if (!request.getPathInfo().empty())
		varsNumber++;
	if (!request.getQuery().empty())
		varsNumber++;
	if (!request.getContentType().empty())
		varsNumber++;
	if (!request.getContentLength().empty())
		varsNumber++;

	_env = new char*[varsNumber + params.size()];

	_env[0] = setEnvParam("SERVER_SOFTWARE=webserver");
	_env[1] = setEnvParam("SERVER_NAME=" + request.getHost());
	_env[2] = setEnvParam("GATEWAY_INTERFACE=CGI/1.1");
	_env[3] = setEnvParam("SERVER_PROTOCOL=HTTP/1.1");
	_env[4] = setEnvParam("SERVER_PORT=" + intToString(config.getPort()));
	_env[5] = setEnvParam("REQUEST_METHOD=" + request.getMethod());
	_env[6] = setEnvParam("PATH_TRANSLATED=" + _path + request.getFile());
	_env[7] = setEnvParam("SCRIPT_NAME=" + _path + request. getFile());
	_env[8] = setEnvParam("REMOTE_ADDR=" + request.getClientIp());

	varsNumber = 9;
	if (!request.getPathInfo().empty())
		_env[varsNumber++] = setEnvParam("PATH_INFO=" +request.getPathInfo());
	if (!request.getQuery().empty())
		_env[varsNumber++] = setEnvParam("QUERY_STRING=" + request.getQuery());
	if (!request.getContentType().empty())
		_env[varsNumber++] = setEnvParam("CONTENT_TYPE=" + request.getContentType());
	if (!request.getContentLength().empty())
		_env[varsNumber++] = setEnvParam("CONTENT_LENGTH=" + request.getContentLength());

	for (std::map<std::string, std::string>::iterator it = params.begin(); it != params.end(); it++)
		_env[varsNumber++] = setEnvParam(toUppercase(it->first) + "=" + it->second);

	_env[varsNumber] = NULL;
}

void CgiHandler::postPipe(int *fd, const std::string& body)
{
	int temp = 0;

	dup2(STDOUT_FILENO, temp);
	pipe(fd);
	dup2(fd[1], STDOUT_FILENO);
	close(fd[1]);
	std::cout << body;
	dup2(temp, STDOUT_FILENO);
}

void CgiHandler::execCgi(int *fdPost, int *fd, const Request& request)
{
	std::string file = request.getFile();
	std::string path = _path + request.getFile();
	char *command[] = {(char *)file.c_str(), NULL};
	if (request.getMethod() == "POST")
	{
		dup2(fdPost[0], STDIN_FILENO);
		close(fdPost[0]);
	}
	dup2(fd[1], STDOUT_FILENO);
	close(fd[1]);
	close(fd[0]);
	execve(path.c_str(), command, _env);
	exit(127);
}

void CgiHandler::exitStatus(const int& pid)
{
	int status;
	int exitCode;

	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
	{
		exitCode = WEXITSTATUS(status);
		if (exitCode == 127)
			_error = "404";
		else if (exitCode != 0)
			_error = "500";
	}
}

void CgiHandler::sendToCgi(const Request& request, const Config& config)//it need time for infinite loop cgi and read in poll
{
	setCgiEnv(request, config);
	pid_t pid;
	int fdPost[2];
	int fdCgi[2];
	char buffer[30720];

	if (request.getMethod() == "POST")
		postPipe(fdPost, request.getBody());
	pipe(fdCgi);
	pid = fork();
	if (pid == -1)
		_error = "500";
	else if (pid == 0)
		execCgi(fdPost, fdCgi, request);
	else
	{
		if (request.getMethod() == "POST")
			close(fdPost[0]);
		close(fdCgi[1]);
		while (read(fdCgi[0], buffer, 30720) > 0)//we cant wait for read and read should be do it with poll or whatever
			_response += buffer;
		if (_response.empty())
			_error = "500";
		close(fdCgi[0]);
		exitStatus(pid);
	}
	freeEnv();
}
