/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmaliare <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 03:05:06 by nmaliare          #+#    #+#             */
/*   Updated: 2023/09/12 05:29:27 by nmaliare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "./../client/Client.hpp"

#include <iostream>
#include <vector>
#include <cstdlib>
#include <string>
#include <sstream>
#include <string.h>

//socket
#include <sys/types.h>
#include <sys/socket.h>

//inet_ntop
#include <arpa/inet.h>

//struct sockaddr_in, htons
#include <netinet/in.h>

//select, fd_set
#include <sys/select.h>

#include <fcntl.h>
#include <errno.h>

// colors
#define BLUE "\x1b[1;36m"
#define YELLOW "\x1b[1;93m"
#define RED "\x1b[1;31m"
#define RES "\x1b[0m"

class Client;

class Server {
public:
	Server(char *port, char *password);
	~Server();

	short	validatePort(char *port);


private:
	int				_mainFd;
	short			_port;
	std::string	_password;

	std::vector <Client *> _clients;

	Server(const Server &s);
	Server &operator=(const Server &s);

	void _select();
};


#endif
