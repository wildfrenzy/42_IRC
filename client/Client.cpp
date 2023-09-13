/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yli <yli@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 02:35:42 by nmaliare          #+#    #+#             */
/*   Updated: 2023/09/13 04:48:04 by nmaliare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client(int fd, Server *s) : _fd(fd), _server(s), _nickName(""), _authenticated(false){}

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
		tokens.push_back(token);
		input.erase(0, pos + deli.size());
		pos = input.find(deli);
	}
	/*const std::string& firstToken = tokens.front();
	for (size_t i = 0; i < firstToken.length(); ++i)
	{
		char c = firstToken[i];
		if (!std::isupper(c)) {
			this->_server->reply(this, "ERR_UNKNOWNCOMMAND", firstToken + " :Unknown command");
			tokens.erase(tokens.begin());
			return tokens;
		}
	}*/
	return tokens;
}

void Client::callExecute(std::vector <std::string> args) {
	if (!args.empty())
	{
		Cmd *cmd = NULL;
		cmd = this->_server->getCommands()[args[0]];
		if (cmd)
			cmd->execute(*this,args);
		else //temporary
		{
			this->_server->reply(this, "ERR_UNKNOWNCOMMAND", args[0] + " :Unknown command");
			/*std::string msg;
			for (unsigned long i = 0; i < args.size(); ++i) {
				msg.append(args[i]);
			}
			this->_server->reply(this->_server->getClients(), "", msg);*/
		}
		args.erase(args.begin());
	}
	/*else
		std::cout << "no args" << std::endl; //remove later*/
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

std::string Client::getNickName(void)
{
	return this->_nickName;
}

void	Client::setNickName(const std::string& nickName)
{
	this->_nickName = nickName;
}