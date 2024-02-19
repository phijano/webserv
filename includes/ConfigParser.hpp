/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vnaslund <vnaslund@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 13:42:17 by vnaslund          #+#    #+#             */
/*   Updated: 2024/02/19 19:43:09 by vnaslund         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGPARSER_HPP
# define CONFIGPARSER_HPP

# include <string>
# include <vector>
# include <fstream>
# include <sstream>

class Config;

class ConfigParser
{
	public:
		ConfigParser();
		~ConfigParser();

		void	parseFile(const std::string& fileName);
		bool	isValidConfigFile(const std::string& fileName);
		void	removeWhitespace(std::string& content);
		void	splitServers(std::string& content);
		size_t	findStartOfServer(size_t start, std::string& content);
		size_t	findEndOfServer(size_t start, std::string& content);

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