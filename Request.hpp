/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phijano- <phijano-@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 10:19:12 by phijano-          #+#    #+#             */
/*   Updated: 2024/02/09 12:08:55 by phijano-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
 #define REQUEST_HPP

#include <string>
#include <vector>
#include <iostream>
#include <sstream>

class Request
{
	public:

		Request();
		Request(std::string request);
		Request(const Request &other);
		Request &operator=(const Request &other);
		~Request();

		std::string 							getMethod() const;
		std::string 							getPath() const;
		std::vector<std::vector<std::string> >	getParameters() const;
		bool	 								getError() const;

		void									parseRequest(std::string request);

	private:

		std::string 							_method;
		std::string 							_path;
		std::vector<std::vector<std::string> >	_parameters;
		bool									_error;
		
		void parseParameter(std::string formField);
};

#endif
