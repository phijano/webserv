/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiHandler.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phijano- <phijano-@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 12:44:14 by phijano-          #+#    #+#             */
/*   Updated: 2024/03/05 12:57:26 by phijano-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CgiHandler.hpp"

CgiHandler::CgiHandler()
{
}

CgiHandler::CgiHandler(Request request, Config config, std::string path)
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

char* CgiHandler::setEnvParam(std::string param)
{
	char *temp = new char[param.size() + 1];
	strcpy (temp, param.c_str());
	return temp;
}

std::string CgiHandler::intToString(int number)
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

void CgiHandler::setCgiEnv(Request request, Config config)
{
	std::map<std::string, std::string> params = request.getCgiHeaderParams();
	_env = new char*[15 + params.size()];

	_env[0] = setEnvParam("SERVER_SOFTWARE=webserver");
	_env[1] = setEnvParam("SERVER_NAME=" + request.getHost());
	_env[2] = setEnvParam("GATEWAY_INTERFACE=CGI/1.1");
	_env[3] = setEnvParam("SERVER_PROTOCOL=HTTP/1.1");
	_env[4] = setEnvParam("SERVER_PORT=" + intToString(config.getPort()));
	_env[5] = setEnvParam("REQUEST_METHOD=" + request.getMethod());
	_env[6] = setEnvParam("PATH_INFO=" +request.getPathInfo());
	_env[7] = setEnvParam("PATH_TRANSLATED=" + _path + request.getFile());
	_env[8] = setEnvParam("SCRIPT_NAME=" + _path + request. getFile());
	_env[9] = setEnvParam("QUERY_STRING=" + request.getQuery());
	_env[10] = setEnvParam("REMOTE_HOST=");
	_env[11] = setEnvParam("REMOTE_ADDR=" + request.getClientIp());
	_env[12] = setEnvParam("CONTENT_TYPE=" + request.getContentType());
	_env[13] = setEnvParam("CONTENT_LENGTH=" + request.getContentLength());

	int i = 14;
	for (std::map<std::string, std::string>::iterator it = params.begin(); it != params.end(); it++)
	{
		_env[i] = setEnvParam(toUppercase(it->first) + "=" + it->second);
		i++;
	}
	_env[i] = NULL;
}

void CgiHandler::postPipe(int *fd, std::string body)
{
	int temp = 0;

	dup2(STDOUT_FILENO, temp);
	pipe(fd);
	dup2(fd[1], STDOUT_FILENO);
	close(fd[1]);
	std::cout << body;
	dup2(temp, STDOUT_FILENO);
}

void CgiHandler::execCgi(int *fdPost, int *fd, Request request)
{
	std::string file = request.getFile();
	std::string path = _path + request.getFile();
	char *command[] = {(char *)file.c_str(), NULL};
	int error;
	if (request.getMethod() == "POST")
	{
		dup2(fdPost[0], STDIN_FILENO);
		close(fdPost[0]);
	}
	dup2(fd[1], STDOUT_FILENO);
	close(fd[1]);
	close(fd[0]);
	error = execve(path.c_str(), command, _env);
	exit(1);
}

void CgiHandler::sendToCgi(Request request, Config config)//it need time for infinite loop cgi and read in poll
{
	std::cout << "CGI" << std::endl;

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
		int status;
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
		{
			int exitCode = WEXITSTATUS(status);
			if (exitCode != 0)
				_error = "500";
		}
		_response = buffer;
		close(fdCgi[0]);
	}
	freeEnv();
}
