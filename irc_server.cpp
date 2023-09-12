/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc_server.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmaliare <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/09 23:06:08 by nmaliare          #+#    #+#             */
/*   Updated: 2023/09/12 02:17:40 by nmaliare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <vector>
#include <cstdlib>
#include <string>
#include <istream>
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

// colors
#define BLUE "\x1b[1;36m"
#define YELLOW "\x1b[1;93m"
#define RED "\x1b[1;31m"
#define RES "\x1b[0m"

class Client{
public:
	Client(int fd);
	~Client();
	std::string _writeBuff;
	std::string _readBuff;
	int _fd;
	std::string _host;
};

Client::Client(int fd) : _fd(fd) {}
Client::~Client() {}

int main(int ac, char *av[]) {
	int mainfd, clientnum = 0;
	//std::vector<int> fds;


	if (ac < 3) {
		std::cout << RED"USAGE: ./irc <port> <pass>"RES << std::endl;
		return 0;
	}
	char *end;
	short port = static_cast <short>(std::strtol(av[1], &end, 10));
	std::cout << BLUE"PORT: "RES << port << std::endl;

	// Creating socket fd:    ipv4         TCP             IP
	if ((mainfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		std::cout << RED"socket failed"RES << std::endl; //throw it from class
		return 0;
	}
	//fds.push_back(mainfd); //fds[0] always main
	//clientnum++;
/*	 setting up reuse of port or address for our socket
	 to avoid err "addr is already in use"*/
	int opt = 1;
	if (setsockopt(mainfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt,sizeof(int))) {
		std::cout << RED"setsockopt error"RES << std::endl; //throw it from class
		return 0;
	}
	struct sockaddr_in address;
/*	 sin_family: The address family for the transport address.
	 This element should always be set to AF_INET.
	 htons onverts the unsigned short integer hostshort
	 from host byte order to network byte order.*/
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY; //any IP
	address.sin_port = htons(port);

	if (bind(mainfd, (struct sockaddr *) &address, sizeof(address)) < 0) {
		std::cout << RED"bind failed"RES << std::endl; //throw it from class
		return 0;
	}
/*	 The backlog(n) argument defines the maximum length to which  the  queue
	       of pending connections for sockfd may grow
	 usually in case the server stops responding for long to stack clients*/
	if (listen(mainfd, 16) < 0) {
		std::cout << RED"listen failed"RES << std::endl; //throw it from class
		return 0;
	}

	/*************	select()	*****************************/

	fd_set r, w; //read, write
	int maxFd = mainfd;
	std::vector < Client * > clients;
	int newfd, addrlen = sizeof(address);

	char buf[11];
	size_t bytes;
	std::string tmp;

	while (42) {
		FD_ZERO(&r);
		FD_ZERO(&w);
		FD_SET(mainfd, &r);
		maxFd = mainfd;
		for (int i = 0; i < clientnum; ++i) {
			maxFd = maxFd < clients[i]->_fd ? clients[i]->_fd : maxFd;
			FD_SET(clients[i]->_fd, &r);
			if (!(clients[i]->_writeBuff.empty())) {
				FD_SET(clients[i]->_fd, &w);
			}
		}
		if (select(maxFd + 1, &r, &w, 0, 0) == -1) {
			std::cout << RED"select fail"RES << std::endl; //throw it from class
			return 0;
		}
		if (FD_ISSET(mainfd, &r)) {
			if ((newfd = accept(mainfd,
								(struct sockaddr *) &address,
								(socklen_t * ) & addrlen)) < 0) {
				std::cout << RED"accept failed"RES
						  << std::endl; //throw it from class
				return 0;
			}
			clientnum++;
			clients.push_back(new Client(newfd));
			clients[clientnum - 1]->_host = inet_ntoa(address.sin_addr);
			std::cout << BLUE "Client [" << clientnum - 1 << "] "RES
					  << "arrived!" << std::endl;
		}
		for (int i = 0; i < clientnum; ++i) {
			if (FD_ISSET(clients[i]->_fd, &r)) {
				fcntl(clients[i]->_fd, F_SETFL, O_NONBLOCK);
				while ((bytes = recv(clients[i]->_fd, buf, 10, 0)) == 10) {
					buf[bytes] = 0;
					clients[i]->_readBuff.append(buf);
					bzero(buf, 10);
				}
				if (bytes <= 0) {
					std::cout << RED "connection closed during recv()" RES
							  << std::endl;
					//clean all
					return 0;
				}
				else if (bytes > 0 && bytes < 10) {
					std::cout << "bytes: " << bytes << std::endl;;
					buf[bytes] = 0;
					clients[i]->_readBuff.append(buf);
				}

				std::cout << YELLOW"CLIENT [" << i << "]: " RES;
				std::cout << clients[i]->_readBuff;

				// temporary sending to all clients everything:
				std::istringstream stream(clients[i]->_readBuff);
				while (std::getline(stream, tmp)) {
					clients[i]->_readBuff = clients[i]->_readBuff.substr(
							clients[i]->_readBuff.find(
									'\n') + 1);
					for (int j = 0; j < clientnum; ++j) {
						clients[j]->_writeBuff.append(tmp);
					}
				}
				// parse command
				// exec command and write result in _write buff
				continue;
			}
			if (FD_ISSET(clients[i]->_fd, &w)) {
				send(clients[i]->_fd, "\x1b[1;93m", 7, 0); //YELLOW
				bytes = send(clients[i]->_fd, clients[i]->_writeBuff.c_str(),
							 clients[i]->_writeBuff.size(), 0);
				send(clients[i]->_fd, "\n", 1, 0);
				send(clients[i]->_fd, "\x1b[0m", 4, 0); //RES
				if (bytes == clients[i]->_writeBuff.size()) {
					clients[i]->_writeBuff.clear();
				}
				else if (bytes != clients[i]->_writeBuff.size() && bytes > 0) {
					clients[i]->_writeBuff = clients[i]->_writeBuff.substr(
							clients[i]->_writeBuff[bytes]);
				}
				else if (bytes <= 0) {
					std::cout << RED "connection closed during send()" RES
							  << std::endl;
					//clean all
					return 0;
				}
			}
		}
	}
}