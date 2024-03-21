/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vnaslund <vnaslund@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 17:01:56 by vnaslund          #+#    #+#             */
/*   Updated: 2024/03/21 16:57:23 by vnaslund         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_H
# define LOCATION_H

# include <vector>
# include <iostream>

class Location
{
	public:
		Location();
		~Location();

		//Getters
		std::string					getPath();
		std::string 				getRoot();
		std::string 				getIndex();
		std::string 				getCgiExt();
		std::string 				getCgiPath();
		std::vector<std::string>	getAllowedMethods();
		std::string					getUploadedPath();
		bool						getAutoIndex();
		bool						getAllowUploads();
		
		//Setters
		void	setPath(const std::string newPath);
		void	setRoot(const std::string newRoot);
		void	setIndex(const std::string newIndex);
		void	setCgiExt(const std::string newCgiExt);
		void	setCgiPath(const std::string newCgiPath);
		void	addAllowedMethod(std::string newMethod);
		void	setUploadedPath(const std::string newUploadedPath);
		void	setAutoIndex(bool newAutoIndex);
		void	setAllowUploads(bool newAllowUploads);
	
		friend std::ostream& operator<<(std::ostream& os, Location& location);

	private:
		std::string 				path;
		std::string 				root;
		std::string 				index;
		std::string 				cgiExt;
		std::string 				cgiPath;
		std::vector<std::string>	allowedMethods;
		std::string					uploadedPath;
		bool						autoIndex;
		bool						allowUploads;
};

#endif