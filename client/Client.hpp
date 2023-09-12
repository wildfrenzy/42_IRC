/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmaliare <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 02:35:42 by nmaliare          #+#    #+#             */
/*   Updated: 2023/09/12 03:10:49 by nmaliare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include "../server/Server.hpp"

class Client {
public:
	Client(int fd, Server *s);
	~Client();

	std::string &getWriteBuff();
	void setWriteBuff(std::string &w);

	std::string &getReadBuff();
	void setReadBuff(std::string &r);

	int getFd() const;

	std::string const &getHost() const;
	void setHost(std::string &host);

	Server *getServer() const;

	/**	other getters setters **/

private:
	Server *_server;

	int _fd;

	std::string _writeBuff;
	std::string _readBuff;
	std::string _host;

	Client(Client &c);
	Client &operator=(Client &c);
};


#endif
