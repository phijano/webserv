/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vnaslund <vnaslund@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 10:19:12 by phijano-          #+#    #+#             */
/*   Updated: 2024/02/21 14:37:21 by phijano-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP

# include <string>
# include <vector>
# include <iostream>
# include <sstream>

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
		std::string								getFile() const;
		std::string								getQuery() const;
		std::string								getContentType() const;
		std::string								getContentLength() const;
		std::string								getBody() const;
		bool	 								getError() const;

	private:

		std::string 							_method;
		std::string 							_path;
		std::string								_file;
		std::string								_query;
		std::string								_contentType;
		std::string								_contentLength;
		std::string								_body;
		bool									_error;
	
		void parseRequest(std::string request);
		void parseHeader(std::string header);
		void parseUrl(std::string url);
};

#endif
