/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmaliare <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/24 16:31:20 by nmaliare          #+#    #+#             */
/*   Updated: 2023/10/04 17:50:48 by nmaliare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <unistd.h>
#include <climits>
#include <stdint.h>

extern Server *ptr;
bool g_cleaned;

Server::Server(char *port, char *password){
	int opt = 1;

	ptr = this;
	g_cleaned = false;
	this->_port = this->validatePort(port);
	this->_password = this->checkPassword(password);

	if ((this->_mainFd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		throw std::runtime_error("irc server socket: " + std::string(strerror(errno)));
	if (setsockopt(this->_mainFd, SOL_SOCKET,  SO_REUSEADDR | SO_REUSEPORT, &opt,sizeof(int)))
		throw std::runtime_error("irc server setsockopt: " + std::string(strerror(errno)));

	fcntl(this->_mainFd, F_SETFL, O_NONBLOCK);

	struct sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(this->_port);

	std::cout << BLUE"PORT: "RES << this->_port << std::endl;

	if (bind(this->_mainFd, (struct sockaddr *) &address, sizeof(address)) < 0)
		throw std::runtime_error("irc server bind: " + std::string(strerror(errno)));

	if (listen(this->_mainFd, 16) < 0)
		throw std::runtime_error("irc server listen: " + std::string(strerror(errno)));

	this->_createCommands();
	this->_setReplies();
	this->_select();
}

void Server::_signalCatch(int signum) {
	if (signum == SIGINT)
	{
		ptr->_cleaner();
		exit(130);
	}
	else if (signum == SIGQUIT)
	{
		ptr->_cleaner();
		exit(131);
	}
}

void	Server::_cleaner()
{
	if (g_cleaned){
		Server::~Server();
		return;
	}
	for (std::vector <Client *>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
		close((*it)->getFd());
		(*it)->getWriteBuff().clear();
		(*it)->getReadBuff().clear();
		delete (*it);
	}
	for (std::map<std::string, Cmd *>::iterator it = _commands.begin(); it != _commands.end(); ++it) {
		delete it->second;
	}
	for (std::map<std::string, Channel *>::iterator it = _channels.begin(); it != _channels.end(); ++it) {
		delete it->second;
	}
	_clients.clear();
	_commands.clear();
	_channels.clear();
	_replies.erase(_replies.begin(), _replies.end());
	_clients.erase(_clients.begin(), _clients.end());
	if (close(_mainFd) == -1){
		throw std::runtime_error("irc server close: " + std::string(strerror(errno)));
	}
	g_cleaned = true;
	Server::~Server();
}

Server::~Server() {
	if (!g_cleaned)
		_cleaner();
}

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

uint16_t Server::validatePort(char *port) {
	char *end;
	long pp = std::strtol(port, &end, 10);
	if (pp > UINT16_MAX || pp < 0)
		throw std::runtime_error("irc server: Ports must be unsigned 16-bit integers (0-65535)");
	uint16_t p = static_cast <uint16_t>(pp);
	if (p <= 1023)
		throw std::runtime_error("irc server: Port numbers from 0 to 1023 are reserved for common TCP/IP applications");
	return p;
}

void Server::_select() {
	fd_set r, w; //read, write
	int newfd, maxFd = this->_mainFd;
	struct sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(this->_port);

	int clientnum = 0, addrlen = sizeof(address);

	char buf[11];
	size_t bytes;
	std::string tmp;

	while (42)
	{
		signal(SIGINT, _signalCatch);
		signal(SIGQUIT, _signalCatch);

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
		if (select(maxFd + 1, &r, &w, 0, 0) == -1){
			_cleaner();
			throw std::runtime_error("irc server select: " + std::string(strerror(errno)));
		}

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
					this->_clients[i]->getReadBuff().append(buf);
					bzero(buf, 10);
				}
				if (bytes <= 0) {
					std::cout << RED "Connection to Client[";
					if (this->_clients[i]->getNickName().empty())
						std::cout << i;
					else
						std::cout << this->_clients[i]->getNickName();
					std::cout << "] closed" RES << std::endl;
					close(this->_clients[i]->getFd());
					deleteClient(this->_clients[i]);
					clientnum--;
					break ;
				}
				else if (bytes > 0 && bytes < 10) {
					buf[bytes] = 0;
					this->_clients[i]->getReadBuff().append(buf);
				}

				// executing commands
				if (this->_clients[i]->getReadBuff().find('\r') != std::string::npos) {
					std::istringstream stream(this->_clients[i]->getReadBuff());
					std::string rb;
					while (std::getline(stream, tmp)) {
						rb = this->_clients[i]->getReadBuff();
						rb = rb.substr(rb.find('\n') + 1);
						this->_clients[i]->setReadBuff(rb);
						_printClient(i);
						this->_clients[i]->callExecute(this->_clients[i]->cmdTokens(
								tmp));
					}
					continue;
				}
			}
			if (FD_ISSET(this->_clients[i]->getFd(), &w)){
				bytes = send(this->_clients[i]->getFd(), this->_clients[i]->getWriteBuff().c_str(),
							 this->_clients[i]->getWriteBuff().size(), 0);
				std::string wb;
				if (bytes == this->_clients[i]->getWriteBuff().size()) {
					this->_clients[i]->getWriteBuff().clear();
					this->_clients[i]->getReadBuff().clear();
				}
				else if (bytes != this->_clients[i]->getWriteBuff().size() && bytes > 0) {
					wb = this->_clients[i]->getWriteBuff();
					wb = wb.substr(wb[bytes]);
					this->_clients[i]->setWriteBuff(wb);
				}
				else if (bytes <= 0) {
					close(this->_clients[i]->getFd());
					deleteClient(this->_clients[i]);
					std::cout << RED "Connection to Client["<< i << "] closed" RES << std::endl;
					clientnum--;
					break ;
				}
			}
		}
	}
}

void Server::deleteClient(Client *c) {
	std::map <std::string, Channel *>::iterator i;
	for (i = _channels.begin() ; i != _channels.end() ; ++i) {
		(*i).second->deleteOperator(c);
		(*i).second->deleteMembers(c);
		(*i).second->removeInvitee(c);
	}
	this->_clients.erase(std::find(this->_clients.begin(),
								   this->_clients.end(), c));
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
	this->_channels[name]->setChannelName(name);
	std::cout << "New channel just appeared : " << name << std::endl;
}

void Server::replyNoServ(Client *who, std::string msg) {
	std::string writeBuff = who->getWriteBuff();

	std::string message = msg + "\r\n";
	std::cout << BLUE"MESSAGE: "RES << message;
	who->setWriteBuff(writeBuff.append(message));
}

void Server::reply(Client *who, std::string reply, std::string msg) {
	std::string writeBuff = who->getWriteBuff();

	std::string message = ":irc_server" + (reply.empty() ? "" : " " + this->_replies[reply] + " ");
	if (!reply.empty())
		message += who->getNickName().empty() ? "[noNickname]" :  who->getNickName();
	message += " " + msg + "\r\n";
	std::cout << BLUE"MESSAGE: "RES << message;
	who->setWriteBuff(writeBuff.append(message));
}

void Server::reply(std::vector<Client *> clients, std::string reply, std::string msg) {
	for (unsigned long i = 0; i < clients.size(); ++i) {
		this->reply(clients[i], reply, msg);
	}
}

void Server::replyNoServ(std::vector<Client *> clients, std::string msg) {
	for (unsigned long i = 0; i < clients.size(); ++i) {
		this->replyNoServ(clients[i], msg);
	}
}

void Server::_createCommands() {
	this->_commands["PASS"] = new Pass();
	this->_commands["USER"] = new User();
	this->_commands["NICK"] = new Nick();
	this->_commands["JOIN"] = new Join();
	this->_commands["KICK"] = new Kick();
	this->_commands["INVITE"] = new Invite();
	this->_commands["PRIVMSG"] = new Privmsg();
	this->_commands["TOPIC"] = new Topic();
	this->_commands["MODE"] = new Mode();
	this->_commands["PING"] = new Ping();
	this->_commands["BOT"] = new Bot();
}

void Server::_setReplies() {

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

	this->_replies["ERR_UNKNOWNERROR"] = "400";
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
	this->_replies["RPL_CHANNELMODEIS"] = "324";

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

	this->_replies["ERR_NOORIGIN"] = "409";
	this->_replies["ERR_UNKNOWNCOMMAND"] = "421";

}

void	Server::replyTime(Client *who, std::vector<Client *> clients, std::string msg, std::string channelname)
{
	char buffer[80];

	std::time_t now = std::time(0);
	std::strftime(buffer, sizeof(buffer), "%s", std::localtime(&now));
	std::string strTime(buffer);
	std::string message;
	message += channelname + " " + who->getNickName() + "!" + who->getUserName() + "@" + who->getHost() + " " + strTime;
	this->reply(clients, msg, message);
}

std::string const Server::checkPassword(char *pass)
{
	std::string const pw = pass;
	for (std::string::const_iterator it = pw.begin(); it != pw.end(); ++it)
	{
		if (!std::isprint(*it))
			throw std::runtime_error("irc server: Password must include only printable characters");
	}
	return pw;
}

void Server::_printClient(int i) {
	std::cout << YELLOW"CLIENT [";
	if (this->_clients[i]->getNickName().empty())
		std::cout << i;
	else
		std::cout << this->_clients[i]->getNickName();
	std::cout << "] " RES << std::endl;
}