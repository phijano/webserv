/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vnaslund <vnaslund@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 09:49:39 by phijano-          #+#    #+#             */
/*   Updated: 2024/02/20 18:02:21 by vnaslund         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "ConfigParser.hpp"

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		std::cerr << "Please provide exactly 1 .conf file" << std::endl;
		return (1);
	}
	try
	{
		ConfigParser	parser;
		parser.parseFile(argv[1]);
	}
	catch (const ConfigParser::ConfigFileException& e) 
		{std::cerr << e.what() << std::endl;}
	
	//Server test2 = Server("0.0.0.0", 8080);
	//(void)test2;
	//std::cout << "Hello webserv" << std::endl;
	
	return 0;
}
