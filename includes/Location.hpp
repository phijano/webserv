/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbengoec <pbengoec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 17:01:56 by vnaslund          #+#    #+#             */
/*   Updated: 2024/04/15 19:49:00 by pbengoec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_H
# define LOCATION_H
# include "Library.hpp"
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
		std::string					getUploadPath();
		bool						getAutoIndex();
		bool						getAllowUploads();
		
		//Setters
		void	setPath(const std::string newPath);
		void	setRoot(const std::string newRoot);
		void	setIndex(const std::string newIndex);
		void	setCgiExt(const std::string newCgiExt);
		void	setCgiPath(const std::string newCgiPath);
		void	addAllowedMethod(std::string newMethod);
		void	setUploadPath(const std::string newUploadedPath);
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
		std::string					uploadPath;
		bool						autoIndex;
		bool						allowUploads;
};

#endif
