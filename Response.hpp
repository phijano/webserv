/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vnaslund <vnaslund@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 10:48:47 by phijano-          #+#    #+#             */
/*   Updated: 2024/02/16 18:09:02 by vnaslund         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include <unistd.h>
# include <fstream>
# include "Request.hpp"

class Response
{
	public:

		Response();
		Response(Request request);
		Response(const Response &other);
		Response &operator=(const Response &other);
		~Response();

		std::string	getResponse() const;

	private:

		std::string	_protocol;
		std::string	_code;
		std::string	_mime;
		std::string	_body;
		std::string _cgiResponse;

		std::string getExtension(std::string file);

		void	getCode(std::string code);
		void	getMime(std::string file);
		void	getErrorPage(std::string error);

		void getMethod(std::string path, std::string file);
		void postMethod(std::string path, std::string file, std::vector<std::vector<std::string> > parameters);
		void deleteMethod(std::string path, std::string file);

		void execCgi(int *fd, std::string path, std::string file, char **env);
		void sendToCgi(void);
};

#endif
