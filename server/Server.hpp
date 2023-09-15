/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yli <yli@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 03:05:06 by nmaliare          #+#    #+#             */
/*   Updated: 2023/09/15 14:49:03 by yli              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

// /connect -nocap -notls -rawlog ~/Code/IRC/myIRC/log.log localhost 6667

#include "./../client/Client.hpp"
#include "./../cmd/Cmd.hpp"
#include "./../commands/pass/Pass.hpp"
#include "./../commands/user/User.hpp"
#include "./../commands/nick/Nick.hpp"
#include "./../commands/join/Join.hpp"
#include "./../commands/invite/Invite.hpp"
#include "./../commands/privmsg/Privmsg.hpp"
#include "./../commands/mode/Mode.hpp"
#include "./../commands/topic/Topic.hpp"
#include "./../commands/kick/Kick.hpp"


#include "./../channel/Channel.hpp"

#include <iostream>
#include <vector>
#include <cstdlib>
#include <string>
#include <sstream>
#include <map>
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

//unix timestamp
#include <ctime>

// colors
#define BLUE "\x1b[1;36m"
#define YELLOW "\x1b[1;93m"
#define RED "\x1b[1;31m"
#define RES "\x1b[0m"

class Client;
class Cmd;
class Channel;

class Server {
public:
	Server(char *port, char *password);
	~Server();

	short	validatePort(char *port);
	void	createComands();


	std::vector <Client *> const &getClients() const;
	std::map <std::string, Cmd *> &getCommands();
	std::string const &getPass() const;
	std::map <std::string, Channel *> &getChannels();
	void addChannel(std::string name);

	void	reply(Client *who, std::string reply, std::string msg);
	void	reply(std::vector <Client *> _clients, std::string reply, std::string msg);
	void	replyTime(Client *who, std::string msg, std::string channel, std::string nickname, std::time_t currentTime);

private:
	int				_mainFd;
	short			_port;
	std::string		_password;

	std::vector <Client *>				_clients;
	std::map <std::string, std::string>	_replies;
	std::map <std::string, Cmd *>		_commands;
	std::map <std::string, Channel *>	_channels;

	Server(const Server &s);
	Server &operator=(const Server &s);

	void	_select();
	void	_setReplies();
	void	_createComands();
};


#endif