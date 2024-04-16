/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vnaslund <vnaslund@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 18:51:14 by vnaslund          #+#    #+#             */
/*   Updated: 2024/04/16 19:22:30 by vnaslund         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include "Library.hpp"
# include "Location.hpp"
# include "Request.hpp"
# include "Config.hpp"
# include "CgiHandler.hpp"

class Response
{
	public:
		Response();
		Response(Request& request, Config& config);
		Response(const Response& other);
		Response &operator=(const Response& other);
		~Response();

		std::string	getResponse() const;

	private:
		Location	_location;

		std::string	_protocol;
		std::string	_code;
		std::string	_mime;
		std::string	_body;
		std::string _cgiResponse;
		bool		_indexNotFound;

		std::string getExtension(const std::string& file);

		void	setCode(const std::string& code);
		void	setMime(const std::string& file);
		void	getErrorPage(const Config& config, const std::string error);

		bool	wrongHost(std::string host, std::vector<std::string> validHosts);
		Location getRequestLocation(const Request& request, Config& config);
		std::string getPath(const Request& request, const Config& config);
		std::string getIndex(const Config& config);
		bool isAllowedMethod(const std::string& method);
		void getMethod(const Request& request, const Config& config);
		int	 createFile(const std::string& file, const std::string& content, const Config& config);
		void upload(const Request& request, const Config& config);
		void postMethod(const Request& request, const Config& config);
		void deleteMethod(const Request& request, const Config& config);
		std::string createIndex(std::string fullPath, std::string path);
};

#endif
