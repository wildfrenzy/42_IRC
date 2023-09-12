/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmaliare <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 02:35:42 by nmaliare          #+#    #+#             */
/*   Updated: 2023/09/12 05:18:53 by nmaliare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include "./../server/Server.hpp"

class Server;

class Client {
public:
	Client(int fd, Server *s);
	Client(Client &c);
	~Client();

	std::string &getWriteBuff();
	void setWriteBuff(std::string &w);

	std::string &getReadBuff();
	void setReadBuff(std::string &r);

	int getFd() const;

	std::string const &getHost() const;
	void setHost(std::string const &host);

	Server *getServer() const;

	/**	other getters setters **/

private:
	int _fd;
	Server *_server;

	std::string _writeBuff;
	std::string _readBuff;
	std::string _host;

	Client &operator=(Client &c);
};


#endif
