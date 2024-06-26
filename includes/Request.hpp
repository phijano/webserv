/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbengoec <pbengoec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 10:19:12 by phijano-          #+#    #+#             */
/*   Updated: 2024/04/04 11:30:52 by phijano-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP

# include "Library.hpp"

class Request
{
	public:

		Request();
		Request(const std::string& request);
		Request(const Request &other);
		Request &operator=(const Request &other);
		~Request();

		void								setClientIp(unsigned long clientIp);
		std::string							getClientIp() const;
		std::string 						getMethod() const;
		std::string 						getPath() const;
		std::string							getFile() const;
		std::string							getQuery() const;
		std::string							getPathInfo() const;
		std::string							getProtocol() const;
		std::string							getHost() const;
		std::string							getContentType() const;
		std::string							getContentLength() const;
		std::map<std::string, std::string>	getCgiHeaderParams() const;
		std::string							getBody() const;
		bool	 							getError() const;

		friend	std::ostream& operator<<(std::ostream& os, Request& request);

	private:

		std::string _clientIp;
		std::string _method;
		std::string _path;
		std::string	_file;
		std::string _query;
		std::string _pathInfo;
		std::string _protocol;
		std::string _host;
		std::map<std::string, std::string> _cgiHeaderParams;
		std::string _contentType;
		std::string	_contentLength;
		std::string	_body;
		bool		_error;
	
		void parseRequest(const std::string& request);
		void parseFirstLine(const std::string& line);
		void parseHost(const std::string& hostLine);
		void parseHeader(const std::string& header);
		void parseUrl(const std::string& url);
		void checkRequest();
};

#endif
