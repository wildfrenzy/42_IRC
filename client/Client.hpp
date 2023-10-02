/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yli <yli@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 02:35:42 by nmaliare          #+#    #+#             */
/*   Updated: 2023/10/02 19:41:11 by nmaliare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <iostream>
#include <vector>

#include "./../cmd/Cmd.hpp"
#include "./../server/Server.hpp"
#include  "./../bot/Bot.hpp"

class Server;
class Cmd;
class Bot;

class Client {
public:
	Client(int fd, Server *s);
	Client(void);
	Client(Client &c);
	~Client();

	std::vector<std::string> cmdTokens(std::string& input);
	void		callExecute(std::vector<std::string> args);


	std::string &getWriteBuff();
	void setWriteBuff(std::string &w);

	std::string &getReadBuff();
	void setReadBuff(std::string &r);

	int getFd() const;

	std::string const &getHost() const;
	void setHost(std::string const &host);

	Server *getServer() const;

	std::string const &getNickName()const;
	void	setNickName(const std::string& nickName);

	bool const &getAuthenticated() const;
	void setAuthenticated();

	std::string const &getUserName() const;
	void setUserName(std::string &un);

	std::string const &getModes() const;
	void setModes(std::string &m);
	void addModes(std::string m);

	std::string const &getRealName() const;
	void setRealName(std::string &rn);

	bool const &getRegistered() const;
	void setRegistered();

	void setServer(Server* server);
	void setFd(int fd);

private:
	int _fd;
	Server *_server;

	std::string _writeBuff;
	std::string _readBuff;
	std::string _host;
	std::string _nickName;
	std::string _userName;
	std::string _realName;
	std::string _modes;

	bool	_authenticated;
	bool	_registered;

	Client &operator=(Client &c);
};


#endif
