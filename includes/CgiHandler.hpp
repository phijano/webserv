/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiHandler.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phijano- <phijano-@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 12:34:46 by phijano-          #+#    #+#             */
/*   Updated: 2024/03/07 11:42:31 by phijano-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGIHANDLER_HPP
# define CGIHANDLER_HPP

# include <unistd.h>
# include <fstream>
# include <algorithm>
# include "Request.hpp"
# include "Config.hpp"

class CgiHandler
{
	public:

		CgiHandler();
		CgiHandler(const Request& request, const Config& config, const std::string& path);
		CgiHandler(const CgiHandler &other);
		CgiHandler &operator=(const CgiHandler &other);
		~CgiHandler();

		std::string	getResponse() const;
		std::string	getError() const;

	private:

		std::string _path;
		std::string _response;
		std::string _error;	
		char**	_env;


		void sendToCgi(const Request& request, const Config& config);
		void setCgiEnv(const Request& request, const Config& config);
		void postPipe(int *fd, const std::string& body);
		void execCgi(int *fdPost, int *fdCgi, const Request& request);
		void exitStatus(const int& pid);
		std::string intToString(const int& number);
		std::string toUppercase(std::string str);
		char* setEnvParam(const std::string& param);
		void freeEnv();
};

#endif
