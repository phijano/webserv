/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vnaslund <vnaslund@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 13:42:17 by vnaslund          #+#    #+#             */
/*   Updated: 2024/02/19 15:00:15 by vnaslund         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGPARSER_HPP
# define CONFIGPARSER_HPP

# include <string>
# include <fstream>

class ConfigParser
{
	public:
		ConfigParser();
		~ConfigParser();

		void	parseFile(const std::string& fileName);
		bool	isValidConfigFile(const std::string& fileName);

		class ConfigFileException : public std::exception
		{
    		public:
        		ConfigFileException(const std::string& msg);
        		virtual const char* what() const throw();
        		virtual ~ConfigFileException() throw() {}
			
			private:
    			std::string message;
		};
};

#endif