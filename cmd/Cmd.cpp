/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cmd.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yli <yli@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 15:33:39 by yli               #+#    #+#             */
/*   Updated: 2023/09/28 17:33:33 by yli              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cmd.hpp"
#include "./../client/Client.hpp"
Cmd::Cmd() {}

Cmd::~Cmd() {}

Cmd::Cmd(const Cmd &other) {
	*this = other;
}

Cmd &Cmd::operator=(const Cmd &other) {
	(void)other;
	return *this;
}

bool    Cmd::cmdcheck(std::string cmd) const
{
    for(unsigned long i = 0; i < cmd.size(); ++i)
    {
        if (cmd[i] == '#' || cmd[i] == '&' || cmd[i] == '+' || cmd[i] == '@' 
            || cmd[i] == '%' || cmd[i] == '/' || cmd[i] == '*' || cmd[i] == '?' ||
            cmd[i] > 127 || cmd[i] <= 32)
        return true;
    }
    return false;
}

Channel* Cmd::findChannel(Client& who, std::string channelName) const
{
    std::map<std::string, Channel *>::iterator it;
    std::map<std::string, Channel *> channels = who.getServer()->getChannels();
    for(it = channels.begin(); it != channels.end(); ++it)
    {
        if (it->first == channelName)
            return it->second;
    }
    return NULL;
}

Client* Cmd::findClient(Client& who, std::string nickName) const
{
    std::vector<Client*>::const_iterator it;
	Client *cl = NULL;
    for(it = who.getServer()->getClients().begin(); it != who.getServer()->getClients().end(); ++it)
    {
        if ((*it)->getNickName() == nickName)
            return *it;
    }
    return cl;
}

bool    Cmd::intcheck(std::string cmd) const
{
    for (size_t i = 0; i < cmd.size(); ++i)
    {
        if (cmd[i] < '0' || cmd[i] > '9')
            return false;
    }
    return true;
}
