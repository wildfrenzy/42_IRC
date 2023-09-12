/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmaliare <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 02:35:42 by nmaliare          #+#    #+#             */
/*   Updated: 2023/09/12 03:09:44 by nmaliare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client(int fd, Server *s) : _fd(fd), _server(s){}

Client::~Client() {}

Client::Client(Client &c) {
	*this = c;
}

Client &Client::operator=(Client &c) {
	this->_server = c._server;
	this->_writeBuff = c._writeBuff;
	this->_readBuff = c._readBuff;
	this->_host = c._host;
	this->_fd = c._fd;
	return *this;
}

int Client::getFd() const {
	return this->_fd;
}

const std::string &Client::getHost() const {
	return this->_host;
}

void Client::setHost(std::string const &host) {
	this->_host = host;
}

std::string &Client::getReadBuff(){
	return this->_readBuff;
}

void Client::setReadBuff(std::string &r) {
	this->_readBuff = r;
}

std::string &Client::getWriteBuff() {
	return this->_writeBuff;
}

void Client::setWriteBuff(std::string &w) {
	this->_writeBuff = w;
}

Server *Client::getServer() const {
	return this->_server;
}