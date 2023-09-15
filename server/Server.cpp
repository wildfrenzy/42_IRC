/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yli <yli@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 03:05:06 by nmaliare          #+#    #+#             */
/*   Updated: 2023/09/14 16:02:51 by yli              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(char *port, char *password){
	int opt = 1;

	this->_port = this->validatePort(port);
	this->_password = password;

	this->createComands();
	this->_setReplies();

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
					std::cout << RED "Connection to Client[";
					if (this->_clients[i]->getNickName().empty())
						std::cout << i;
					else
						std::cout << this->_clients[i]->getNickName();
					std::cout << "] closed during recv()" RES << std::endl;
					return ;
				}
				else if (bytes > 0 && bytes < 10) {
					buf[bytes] = 0;
					this->_clients[i]->getReadBuff().append(buf);
				}

				std::cout << YELLOW"CLIENT [";
				if (this->_clients[i]->getNickName().empty())
					std::cout << i;
				else
					std::cout << this->_clients[i]->getNickName();
				std::cout << "] " RES << std::endl;

				std::cout << this->_clients[i]->getReadBuff();


			//	std::string message = this->_clients[i]->getReadBuff();
			//	this->_clients[i]->callExecute(this->_clients[i]->cmdTokens(message));

				//TODO Here add :
				// parse command
				// exec command and write result in _write buff

				//TODO remove later everything till line 136(before continue):
				// temporary sending to all clients everything:
				//if (this->_clients[i]->getWriteBuff().empty())
				//{
					//std::cout << "istringstream..." << std::endl;
					std::istringstream stream(this->_clients[i]->getReadBuff());
					std::string rb;
					while (std::getline(stream, tmp)) {
						rb = this->_clients[i]->getReadBuff();
						rb = rb.substr(rb.find('\n') + 1);
						this->_clients[i]->setReadBuff(rb);
						this->_clients[i]->callExecute(this->_clients[i]->cmdTokens(tmp));
					}
				//}

				continue ;
			}
			if (FD_ISSET(this->_clients[i]->getFd(), &w)){
			//	send(this->_clients[i]->getFd(), "\x1b[1;93m", 7, 0); //YELLOW
				bytes = send(this->_clients[i]->getFd(), this->_clients[i]->getWriteBuff().c_str(),
							 this->_clients[i]->getWriteBuff().size(), 0);
				//send(this->_clients[i]->getFd(), "\n", 1, 0);
			//	send(this->_clients[i]->getFd(), "\x1b[0m", 4, 0); //RES

				std::string wb;
				if (bytes == this->_clients[i]->getWriteBuff().size()) {
					this->_clients[i]->getWriteBuff().clear();
					this->_clients[i]->getReadBuff().clear(); //temporary
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

std::map <std::string, Channel *> &Server::getChannels() {
	return this->_channels;
}

std::map<std::string, Cmd *> &Server::getCommands() {
	return this->_commands;
}

const std::string &Server::getPass() const {
	return this->_password;
}

std::vector<Client *> const &Server::getClients() const {
	return this->_clients;
}

void Server::addChannel(std::string name) {
	this->_channels[name] = new Channel();
	std::cout << "New channel just appeared : " << name << std::endl;
}

void Server::reply(Client *who, std::string reply, std::string msg) {
	std::string writeBuff = who->getWriteBuff();
	//in case not everything was sent before

	std::string message = ":irc_server " + (reply.empty() ? "" : this->_replies[reply] + " ");
	if (!reply.empty())
		message += who->getNickName().empty() ? "[noNickname]" :  who->getNickName();
	message += " " + msg + "\r\n";
	std::cout << BLUE"MESSAGE: "RES << message << std::endl;
	who->setWriteBuff(writeBuff.append(message));
}

void Server::reply(std::vector<Client *> clients, std::string reply, std::string msg) {
	for (unsigned long i = 0; i < clients.size(); ++i) {
		this->reply(clients[i], reply, msg);
	}
}

void Server::createComands() {
	this->_commands["PASS"] = new Pass();
	this->_commands["NICK"] = new Nick();
	this->_commands["USER"] = new User();
	this->_commands["JOIN"] = new Join();
}

void Server::_setReplies() {

//:You must authenticate with the server using PASS command first
	this->_replies["ERR_NOTREGISTERED"] = "451";

	this->_replies["RPL_WELCOME"] = "001";
	this->_replies["RPL_YOURHOST"] = "002";
	this->_replies["RPL_CREATED"] = "003";
	this->_replies["RPL_MYINFO"] = "004";

	this->_replies["ERR_NEEDMOREPARAMS"] = "461";
	this->_replies["ERR_ALREADYREGISTERED"] = "462";
	this->_replies["ERR_PASSWDMISMATCH"] = "464";

	this->_replies["ERR_NONICKNAMEGIVEN"] = "431";
	this->_replies["ERR_ERRONEUSNICKNAME"] = "432";
	this->_replies["ERR_NICKNAMEINUSE"] = "433";

	this->_replies["ERR_NOSUCHNICK"] = "401";
	this->_replies["ERR_NOSUCHCHANNEL"] = "403";
	this->_replies["ERR_CANNOTSENDTOCHAN"] = "404";
	this->_replies["ERR_NORECIPIENT"] = "411";
	this->_replies["ERR_NOTEXTTOSEND"] = "412";

	this->_replies["RPL_NOTOPIC"] = "331";
	this->_replies["RPL_TOPIC"] = "332";
	this->_replies["RPL_TOPICWHOTIME"] = "333";

	this->_replies["RPL_INVITELIST"] = "336";
	this->_replies["RPL_NAMREPLY"] = "353";
	this->_replies["RPL_ENDOFNAMES"] = "366";
	this->_replies["ERR_BADCHANNELKEY"] = "475";
	this->_replies["ERR_CHANNELISFULL"] = "471";
	this->_replies["ERR_UNKNOWNMODE"] = "472";
	this->_replies["ERR_INVITEONLYCHAN"] = "473";
	this->_replies["ERR_TOOMANYCHANNELS "] = "405";
	this->_replies["ERR_BADCHANMASK"] = "476";

	this->_replies["RPL_INVITING"] = "341";
	this->_replies["ERR_USERNOTINCHANNEL"] = "441";
	this->_replies["ERR_NOTONCHANNEL"] = "442";
	this->_replies["ERR_USERONCHANNEL"] = "443";
	this->_replies["ERR_NOPRIVILEGES"] = "481";
	this->_replies["ERR_CHANOPRIVSNEEDED"] = "482";

	this->_replies["ERR_USERSDONTMATCH"] = "502";
	this->_replies["RPL_UMODEIS"] = "221";
	this->_replies["ERR_UMODEUNKNOWNFLAG"] = "501";
	this->_replies["RPL_CHANNELMODEIS"] = "324";
	this->_replies["RPL_CREATIONTIME"] = "329";
	//this->_replies["RPL_TOPIC"] = "332";

	this->_replies["ERR_UNKNOWNCOMMAND"] = "421";

/*	this->_replies[""] = "";
	this->_replies[""] = "";
	this->_replies[""] = "";*/

}

void	Server::reply(Client *who, std::string msg, std::string channelname, std::string nickname, std::time currentTime)
{
	std::cout << "<Client>" << " " << channelname << " " << nickname << " " << currentTime << std::endl;
}//check!!!!
