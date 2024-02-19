/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vnaslund <vnaslund@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 10:48:47 by phijano-          #+#    #+#             */
/*   Updated: 2024/02/19 13:30:58 by phijano-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include <unistd.h>
# include <fstream>
# include "Request.hpp"
# include "CgiHandler.hpp" 

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

		void getMethod(Request request);
		void postMethod(Request request);
		void deleteMethod(std::string path, std::string file);
};

#endif
