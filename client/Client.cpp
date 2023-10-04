/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yli <yli@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 02:35:42 by nmaliare          #+#    #+#             */
/*   Updated: 2023/09/22 16:57:47 by yli              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

#define YELLOW "\x1b[1;93m"
#define RES "\x1b[0m"

Client::Client(int fd, Server *s) : _fd(fd), _server(s), _nickName(""),
		_authenticated(false), _registered(false){}

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
	this->_nickName = c._nickName;
	return *this;
}

std::vector <std::string> Client::cmdTokens(std::string &input) {
	std::string deli = " ";
	std::vector<std::string> tokens;
	std::string token;
	size_t pos;
	input += " ";
	pos = input.find(deli);
	while (pos != std::string::npos)
	{
		token = input.substr(0, pos);
		if (token.find('\r') != std::string::npos)
			token = token.substr(0, token.find('\r'));
		if (!token.empty())
			tokens.push_back(token);
		input.erase(0, pos + deli.size());
		pos = input.find(deli);
	}

/*	std::cout << "Tokens check: ";
	for (size_t i = 0; i < tokens.size(); ++i) {
		std::cout << "[" << tokens[i] << "]";
	}
	std::cout << std::endl;*/
	return tokens;
}

void Client::callExecute(std::vector <std::string> args) {
	if (!args.empty())
	{
		Cmd *cmd = NULL;
		cmd = this->_server->getCommands()[args[0]];
		if (cmd)
			cmd->execute(*this,args);
		else
		{
			this->_server->reply(this, "ERR_UNKNOWNCOMMAND", args[0] + " :Unknown command");
		}
	}
}

bool const &Client::getRegistered() const {
	return this->_registered;
}

void Client::setRegistered() {
	if (!this->_registered)
	{
		this->_registered = true;
		std::string msg = YELLOW"Welcome to the IRC Network" + (!this->_nickName.empty() ? ", " + this->_nickName : ", new user") +
				"!" + this->_userName + "@" + this->_host + RES;
		this->_server->reply(this, "RPL_WELCOME", msg);
		this->_server->reply(this, "RPL_YOURHOST",
							 YELLOW"Your host is irc server by nmaliare and yli, running version 1.0"RES);
		this->_server->reply(this, "RPL_CREATED",
							 YELLOW"This server was created on September 10, 2023"RES);
		this->_server->reply(this, "RPL_MYINFO", YELLOW"irc server v1.0 itkol"RES);

	}
	else
		this->_server->reply(this, "ERR_ALREADYREGISTERED", ":You may not reregister");
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

const bool &Client::getAuthenticated() const {
	return this->_authenticated;
}

void Client::setAuthenticated() {
	if (!this->_authenticated)
		this->_authenticated = true;
}

std::string const &Client::getNickName(void) const
{
	return this->_nickName;
}

void	Client::setNickName(const std::string& nickName)
{
	this->_nickName = nickName;
}

std::string const &Client::getUserName() const {
	return this->_userName;
}

void Client::setUserName(std::string &un) {
	this->_userName = un;
}

std::string const &Client::getModes() const {
	return this->_modes;
}

void Client::setModes(std::string &m) {
	this->_modes = m;
}

void Client::addModes(std::string m) {
	this->_modes += m;
}

std::string const &Client::getRealName() const {
	return this->_realName;
}

void Client::setRealName(std::string &rn) {
	this->_realName = rn;
}

/*
std::map <std::string, Channel *> &Client::getUserChannels(){
	return this->_userChannels;
}*/

void Client::setServer(Server* server)
{
	this->_server = server;
}

void Client::setFd(int fd)
{
	this->_fd = fd;
}