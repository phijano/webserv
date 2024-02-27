/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vnaslund <vnaslund@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 17:02:11 by vnaslund          #+#    #+#             */
/*   Updated: 2024/02/27 15:49:02 by vnaslund         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_H
# define CONFIG_H

# include <string>
# include <vector>
# include <map>
# include "Location.hpp"

class Config
{
	public:
		Config();
		~Config();

		//Getters
		std::string					getHost();
		int							getPort();
		int							getBodySize();
		std::string					getRoot();
		std::string					getIndex();
		std::vector<std::string>	getServerNames();
		std::map<int, std::string>	getErrorPages();
		std::vector<Location>		getLocations();

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