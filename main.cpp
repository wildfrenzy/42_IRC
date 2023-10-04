/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmaliare <nmaliare@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 05:09:46 by nmaliare          #+#    #+#             */
/*   Updated: 2023/09/20 22:24:55 by yli              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./server/Server.hpp"
#include <exception>

Server *ptr;

int main(int ac, char *av[]) {
	if (ac < 3) {
		std::cout << RED"USAGE: ./irc <port> <pass>"RES << std::endl;
		return 0;
	}
	try {
		Server server(av[1], av[2]);
	} catch (std::exception const &e) {
		std::cout << e.what() << std::endl;
	}
}
