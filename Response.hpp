/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phijano- <phijano-@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 10:48:47 by phijano-          #+#    #+#             */
/*   Updated: 2024/02/12 13:12:07 by phijano-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
 #define RESPONSE_HPP

#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>
#include "Request.hpp"

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

		void	getCode(std::string code);
		void	getErrorPage(std::string error);

		void getMethod(std::string path);
		void postMethod(std::string path, std::vector<std::vector<std::string> > parameters);
		void deleteMethod(std::string path);
};

#endif
