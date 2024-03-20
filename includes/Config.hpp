/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbengoec <pbengoec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 17:02:11 by vnaslund          #+#    #+#             */
/*   Updated: 2024/03/20 18:19:02 by pbengoec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_H
# define CONFIG_H

# include "Library.hpp"
class Location;
class Config
{
	public:
		Config();
		~Config();

		//Getters
		std::string					getHost() const;
		int							getPort() const;
		int							getBodySize() const;
		std::string					getRoot() const;
		std::string					getIndex() const;
		std::vector<std::string>	getServerNames() const;
		std::map<int, std::string>	getErrorPages() const;
		std::vector<Location>		getLocations() const;

		//Setters
		void	setHost(const std::string newHost);
		void	addServerName(std::string newServerName);
		void 	setPort(int newPort);
		void 	setBodySize(int	newSize);
		void 	setRoot(const std::string newRoot);
		void 	setIndex(const std::string newIndex);
		void 	addErrorPage(int code, std::string path);
		void 	addLocation(Location newLocation);


		friend std::ostream& operator<<(std::ostream& os, Config& config);

	private:
		std::string 				host; // Should host be string or other type ???
		std::vector<std::string>	serverNames;
		int 						port;
		int							bodySize; // unit: M
		std::string 				root;
		std::string 				index;
		std::map<int, std::string>	errors;
    	std::vector<Location>		locations;
};

#endif