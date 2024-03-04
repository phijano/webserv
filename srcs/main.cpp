/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbengoec <pbengoec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 09:49:39 by phijano-          #+#    #+#             */
/*   Updated: 2024/03/04 19:23:35 by pbengoec         ###   ########.fr       */
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
		Server test2 = Server(&servers[0]);
		(void)test2;
	}
	catch (const ConfigParser::ConfigFileException& e) 
		{std::cerr << e.what() << std::endl;}
	return 0;
}
