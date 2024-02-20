/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vnaslund <vnaslund@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 13:42:17 by vnaslund          #+#    #+#             */
/*   Updated: 2024/02/20 18:13:48 by vnaslund         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGPARSER_HPP
# define CONFIGPARSER_HPP

# include <iostream>
# include <vector>
# include <fstream>
# include <sstream>
# include "Config.hpp"

class ConfigParser
{
	public:
		ConfigParser();
		~ConfigParser();

		void						parseFile(const std::string& fileName);
		bool						isValidConfigFile(const std::string& fileName);
		void						removeWhitespace(std::string& content);
		void						splitServers(std::string& content);
		size_t						findStartOfServer(size_t start, std::string& content);
		size_t						findEndOfServer(size_t start, std::string& content);
		void						parseServer(std::string& content, Config server, int serverNum);
		std::vector<std::string>	splitContent(const std::string& content);

		class ConfigFileException : public std::exception
		{
    		public:
        		ConfigFileException(const std::string& msg);
        		virtual const char* what() const throw();
        		virtual ~ConfigFileException() throw() {}
			
			private:
    			std::string message;
		};
	
	private:
		size_t						_nbr_servers;
		std::vector<std::string>	_server_contents;
		std::vector<Config>			_servers;
};

#endif