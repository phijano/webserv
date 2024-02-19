/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiHandler.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phijano- <phijano-@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 12:34:46 by phijano-          #+#    #+#             */
/*   Updated: 2024/02/19 13:25:38 by phijano-         ###   ########.fr       */
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

		void execCgi(int *fd, std::string path, std::string file, char **env);
		void sendToCgi(void);
};

#endif
