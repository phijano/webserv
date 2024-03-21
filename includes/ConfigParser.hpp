/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbengoec <pbengoec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 13:42:17 by vnaslund          #+#    #+#             */
/*   Updated: 2024/03/20 19:02:58 by pbengoec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGPARSER_HPP
# define CONFIGPARSER_HPP
# include "Library.hpp"
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
		void						parseServer(std::string& content, Config& server);
		std::vector<std::string>	splitContent(const std::string& content);

		std::vector<Config>&		getServers(void);

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