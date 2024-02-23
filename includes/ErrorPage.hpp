/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorPage.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vnaslund <vnaslund@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 19:35:02 by vnaslund          #+#    #+#             */
/*   Updated: 2024/02/23 14:28:05 by vnaslund         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERRORPAGE_H
# define ERRORPAGE_H

# include <string>
# include <iostream>

// UNUSED CLASS
class ErrorPage
{
	public:
		ErrorPage();
		ErrorPage(std::string path, int code);
		~ErrorPage();

		//Getters
		std::string getPath();
		int			getCode();

		//Setters
		void	setPath(std::string newPath);
		void	setCode(int	newCode);
	
	friend std::ostream& operator<<(std::ostream& os, const ErrorPage& errorPage);

	private:
		std::string	path;
		int			code;
};

#endif