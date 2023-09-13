/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yli <yli@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 02:35:42 by nmaliare          #+#    #+#             */
/*   Updated: 2023/09/12 19:36:31 by nmaliare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <iostream>
#include <vector>

#include "./../server/Server.hpp"
#include "./../cmd/Cmd.hpp"

class Server;
class Cmd;

class Client {
public:
	Client(int fd, Server *s);
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

	std::string getNickName(void);
	void	setNickName(const std::string& nickName);

	bool const &getAuthenticated() const;
	void setAuthenticated();

	std::string &getUserName() const;
	void setUserName(std::string &un);

	std::string &getModes() const;
	void setModes(std::string &m);

	std::string &getRealName() const;
	void setRealName(std::string &rn);

	//std::map <std::string, Channel *> &getUserChannels();

	/**	other getters setters **/

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

	//std::map <std::string, Channel *>	_userChannels;

	bool	_authenticated;

	Client &operator=(Client &c);
};


#endif
