/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiHandler.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phijano- <phijano-@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 12:34:46 by phijano-          #+#    #+#             */
/*   Updated: 2024/02/21 14:52:41 by phijano-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGIHANDLER_HPP
# define CGIHANDLER_HPP

# include <unistd.h>
# include <fstream>
# include "Request.hpp"

class CgiHandler
{
	public:

		CgiHandler();
		CgiHandler(Request request);
		CgiHandler(const CgiHandler &other);
		CgiHandler &operator=(const CgiHandler &other);
		~CgiHandler();

		std::string	getResponse() const;
		std::string	getError() const;

	private:

		std::string _response;
		std::string _error;	
		char**	_env;


		void sendToCgi(Request request);
		void postFork(int *fd, std::string body);
		void execCgi(int *fdPost, int *fdCgi, Request request);
		void setCgiEnv(Request request);
		char* setEnvParam(std::string param);
		void freeEnv();
};

#endif
