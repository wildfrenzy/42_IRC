/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmaliare <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 03:05:06 by nmaliare          #+#    #+#             */
/*   Updated: 2023/09/12 05:31:37 by nmaliare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(char *port, char *password){
	int opt = 1;

	this->_port = this->validatePort(port);
	this->_password = password;

	if ((this->_mainFd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		throw std::runtime_error("irc server: " + std::string(strerror(errno)));
	if (setsockopt(this->_mainFd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt,sizeof(int)))
		throw std::runtime_error("irc server: " + std::string(strerror(errno)));

	fcntl(this->_mainFd, F_SETFL, O_NONBLOCK);

	struct sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(this->_port);

	std::cout << BLUE"PORT: "RES << this->_port << std::endl;

	if (bind(this->_mainFd, (struct sockaddr *) &address, sizeof(address)) < 0)
		throw std::runtime_error("irc server: " + std::string(strerror(errno)));

	if (listen(this->_mainFd, 16) < 0)
		throw std::runtime_error("irc server: " + std::string(strerror(errno)));

	this->_select();
}

Server::~Server() {}

Server::Server(const Server &s) {
	(void)s;
}

Server &Server::operator=(const Server &s) {
	if (this != &s)
	{
		this->_mainFd = s._mainFd;
		this->_port = s._port;
		this->_password = s._password;
		this->_clients = s._clients;
	}
	return *this;
}

short Server::validatePort(char *port) {
	char *end;
	short p = static_cast <short>(std::strtol(port, &end, 10));
//TODO	add better validation!
// and throw exception on error.
	return p;
}

void Server::_select() {
	fd_set r, w; //read, write
	int newfd, maxFd = this->_mainFd;
	struct sockaddr_in address;
	int clientnum = 0, addrlen = sizeof(address);

	char buf[11];
	size_t bytes;
	std::string tmp; //remove later after we implement commands and responses

	while (42)
	{
		FD_ZERO(&r);
		FD_ZERO(&w);
		FD_SET(this->_mainFd, &r);
		maxFd = this->_mainFd;

		for (int i = 0; i < clientnum; ++i) {
			maxFd = maxFd < this->_clients[i]->getFd() ? this->_clients[i]->getFd() : maxFd;
			FD_SET(this->_clients[i]->getFd(), &r);
			if (!(this->_clients[i]->getWriteBuff().empty())) {
				FD_SET(this->_clients[i]->getFd(), &w);
			}
		}
		if (select(maxFd + 1, &r, &w, 0, 0) == -1)
			throw std::runtime_error("irc server: " + std::string(strerror(errno)));

		if (FD_ISSET(this->_mainFd, &r)){
			if ((newfd = accept(this->_mainFd,(struct sockaddr *) &address,
					(socklen_t * ) & addrlen)) < 0) {
				std::cout << RED"Accepting new connection failed."RES << std::endl;
			}
			else{
				fcntl(newfd, F_SETFL, O_NONBLOCK);
				this->_clients.push_back(new Client(newfd, this));
				this->_clients[clientnum]->setHost(inet_ntoa(address.sin_addr));
				std::cout << BLUE "Client [" << clientnum << "] "RES << "arrived!" << std::endl;
				clientnum++;
			}
		}
		for (int i = 0; i < clientnum; ++i) {
			if (FD_ISSET(this->_clients[i]->getFd(), &r)){
				while ((bytes = recv(this->_clients[i]->getFd(), buf, 10, 0)) == 10) {
					buf[bytes] = 0;
					this->_clients[i]->getReadBuff().append(buf); //_readBuff.append(buf);
					bzero(buf, 10);
				}
				if (bytes <= 0) {
					//clean everything about current client and close fd.
					std::cout << RED "Connection to Client["<< i << "] closed during recv()" RES << std::endl;
					return ;
				}
				else if (bytes > 0 && bytes < 10) {
					buf[bytes] = 0;
					this->_clients[i]->getReadBuff().append(buf);
				}

				std::cout << YELLOW"CLIENT [" << i << "]: " RES;
				std::cout << this->_clients[i]->getReadBuff();

				//TODO Here add :
				// parse command
				// exec command and write result in _write buff

				//TODO remove later everything till line 136(before continue):
				// temporary sending to all clients everything:
				std::istringstream stream(this->_clients[i]->getReadBuff());
				std::string rb;
				while (std::getline(stream, tmp)) {
					rb = this->_clients[i]->getReadBuff();
					rb = rb.substr(rb.find('\n') + 1);
					this->_clients[i]->setReadBuff(rb);
					for (int j = 0; j < clientnum; ++j) {
						this->_clients[j]->getWriteBuff().append(tmp);
					}
				}
				continue ;
			}
			if (FD_ISSET(this->_clients[i]->getFd(), &w)){

				//TODO Here we are ready to write - send response to client.
				// we need to process and send command to execute and send response to client.
				// ***
				// remove later and redo almost everything inside of this if statement:
				// temporary sending same message as I reseived to all clients:

				send(this->_clients[i]->getFd(), "\x1b[1;93m", 7, 0); //YELLOW
				bytes = send(this->_clients[i]->getFd(), this->_clients[i]->getWriteBuff().c_str(),
							 this->_clients[i]->getWriteBuff().size(), 0);
				send(this->_clients[i]->getFd(), "\n", 1, 0);
				send(this->_clients[i]->getFd(), "\x1b[0m", 4, 0); //RES

				std::string wb;
				if (bytes == this->_clients[i]->getWriteBuff().size()) {
					this->_clients[i]->getWriteBuff().clear();
				}
				else if (bytes != this->_clients[i]->getWriteBuff().size() && bytes > 0) {
					wb = this->_clients[i]->getWriteBuff();
					wb = wb.substr(wb[bytes]);
					this->_clients[i]->setWriteBuff(wb);
				}
				else if (bytes <= 0) {
					//clean everything about current client and close fd.
					std::cout << RED "Connection to Client["<< i << "] closed during send()" RES << std::endl;
					return ;
				}
			}
		}
	}
}
