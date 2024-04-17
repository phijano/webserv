/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiHandler.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vnaslund <vnaslund@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 12:44:14 by phijano-          #+#    #+#             */
/*   Updated: 2024/04/17 12:35:02 by phijano-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CgiHandler.hpp"

CgiHandler::CgiHandler() : _env(NULL) 
{
	
}

CgiHandler::CgiHandler(const Request& request, const Config& config, const std::string& path) : _env(NULL)
{
	_path = path;
	if (_path[_path.size() - 1] != '/')
		_path += "/";
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
	_env = NULL;
	if (other._env)
	{
		int i = 0;
		while (other._env[i])
			i++;
		_env = new char*[i];
		i = -1;
		while(other._env[++i])
			_env[i] = setEnvParam(other._env[i]);
 		_env[i] = NULL;
	}
	return *this;
}

CgiHandler::~CgiHandler()
{
	if (_env != NULL)
	{
        for (int i = 0; _env[i] != NULL; ++i) 
            delete[] _env[i];
        delete[] _env;
    }
}

std::string CgiHandler::getResponse() const
{
	return _response;
}

std::string CgiHandler::getError() const
{
	return _error;
}

void CgiHandler::setError(const std::string& error)
{
	_error = error;
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

void CgiHandler::closePipeError(int *fdPipe, const std::string &error)
{
	close(fdPipe[0]);
	close(fdPipe[1]);
	_error = error;
}

void CgiHandler::closeFdError(int fd, const std::string &error)
{
	close(fd);
	_error = error;
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

	if(dup2(STDOUT_FILENO, temp) == -1)
		return setError("500");
	if(pipe(fd) == -1)
		return setError("500");
	if(dup2(fd[1], STDOUT_FILENO) == -1)
		return closePipeError(fd, "500");
	if(close(fd[1]) == -1)
		return closeFdError(fd[0], "500");
	std::cout << body;
	if(dup2(temp, STDOUT_FILENO) == -1)
		return closeFdError(fd[0], "500");
}

void CgiHandler::execCgi(int *fdPost, int *fd, const Request& request)
{
	std::string file = request.getFile();
	std::string path = _path + request.getFile();
	char *command[] = {(char *)file.c_str(), NULL};
	if (request.getMethod() == "POST")
	{
		if(dup2(fdPost[0], STDIN_FILENO) == -1)
			exit(1);
		if(close(fdPost[0]) == -1)
			exit(1);
	}
	if(dup2(fd[1], STDOUT_FILENO) == -1)
		exit(1);
	if(close(fd[1]) == -1)
		exit(1);
	if(close(fd[0]) == -1)
		exit(1);
	exit(execve(path.c_str(), command, _env));
}

void CgiHandler::exitStatus(const int& pid)
{
	int status;
	int exitCode;
	int time;

	time = 0;
	while(waitpid(pid, &status, WNOHANG) != -1)
	{
		if (time == 2000000)
		{
			kill(pid, SIGKILL);
			_error = "508";
			return;
		}
		time++;
	}
	if (WIFEXITED(status))
	{
		exitCode = WEXITSTATUS(status);
		if (exitCode != 0)
			_error = "500";
	}
}

void CgiHandler::initBuffer(char* buffer, unsigned int size)
{
	unsigned int i = 0;

	while (i < size)
		buffer[i++] = '\0';
}

void CgiHandler::sendToCgi(const Request& request, const Config& config)
{
	pid_t pid;
	int fdPost[2];
	int fdCgi[2];
	char buffer[30720];

	initBuffer(buffer, 30720);
	setCgiEnv(request, config);
	if (access((_path + request.getFile()).c_str(), F_OK) == -1)
		return setError("404");
	if (request.getMethod() == "POST")
		postPipe(fdPost, request.getBody());
	if (!_error.empty())
		return ;
	if(pipe(fdCgi) == -1)
		return setError("500");
	pid = fork();
	if (pid == -1)
	{
		if (request.getMethod() == "POST")
			close(fdPost[0]);
		closePipeError(fdCgi, "500");
	}
	else if (pid == 0)
		execCgi(fdPost, fdCgi, request);
	else
	{
		if (request.getMethod() == "POST")
			if (close(fdPost[0]) == -1)
				return closePipeError(fdCgi, "500");

		if (close(fdCgi[1]) == -1)
			return closeFdError(fdCgi[0], "500");
		exitStatus(pid);
		if (!_error.empty())
		{
			close(fdCgi[0]);
			return;
		}
		int bytes = 1;
		while (bytes)
		{
			bytes = read(fdCgi[0], buffer, 30720);
			if (bytes > 0)
				_response += buffer;
			else if (bytes < 0)
				return closeFdError(fdCgi[0], "500");
		}
		if (_response.empty())
			_error = "500";
		if (close(fdCgi[0]) == -1)
			return setError("500");
	}
}
