/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vnaslund <vnaslund@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 09:49:39 by phijano-          #+#    #+#             */
/*   Updated: 2024/02/22 19:16:15 by vnaslund         ###   ########.fr       */
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
		std::vector<Config>	servers = parser.getServers();

		for (size_t i = 0; i < servers.size(); i++)
		{
			// make servers from config class
		}
	}
	catch (const ConfigParser::ConfigFileException& e) 
		{std::cerr << e.what() << std::endl;}
	
	Server test2 = Server("0.0.0.0", 8080);
	(void)test2;
	std::cout << "Hello webserv" << std::endl;
	
	return 0;
}
