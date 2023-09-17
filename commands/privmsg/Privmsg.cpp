/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user <user@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/16 03:09:51 by nmaliare          #+#    #+#             */
/*   Updated: 2023/09/16 12:47:06 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Privmsg.hpp"
#include "./../../client/Client.hpp"

Privmsg::Privmsg(void): Cmd(){}

Privmsg::~Privmsg(void) {}

Privmsg::Privmsg(const Privmsg& other)
{
    *this = other;
}

Privmsg& Privmsg::operator=(const Privmsg& other)
{
	(void)other;
    return *this;
}

void    Privmsg::sendToChannel(Client& who, std::vector<std::string> cmd) const
{
    Channel*    channel = findChannel(who, cmd[1]);
    if (!channel)
    {
        who.getServer()->reply(&who,
                        "ERR_NOSUCHNICK",
                        ":No such nick/channel");
        return ;             
    }   
    std::vector<std::string>::iterator it;
    for (it = cmd.begin() + 2; it < cmd.end(); ++it)
        channel->broadcast(who.getServer(), *it);
}

void    Privmsg::sendToClient(Client& who, std::vector<std::string> cmd) const
{
    Client& client = findClient(who, cmd[1]);
    if (client.getNickName() == who.getServer()->getClients().front()->getNickName())
    {
        who.getServer()->reply(&who,
                        "ERR_NOSUCHNICK",
                        ":No such nick/channel");
        return ;        
    }
    std::vector<std::string>::iterator it;
    // for (it = cmd.begin() + 2; it < cmd.end(); ++it)
    //     client.setWriteBuff((*it));
    for (it = cmd.begin() + 2; it < cmd.end(); ++it)
        client.getServer()->reply(&client, "", *it);    
}


void    Privmsg::execute(Client& who, std::vector<std::string> cmd) const
{
	if (!who.getAuthenticated()){
		who.getServer()->reply(&who,
										 "ERR_ALREADYREGISTERED",
										 ":You may not reregister");
		return;
	}
	if (cmd.size() < 2) {
		who.getServer()->reply(&who,
							   "ERR_NEEDMOREPARAMS",
							   ":Not enough parameters");
		return;
	}
    if (cmd[2][0] != ':')
    {
        who.getServer()->reply(&who,
                            "ERR_UNKNOWNERROR",
                            ":invalid content");
        return;
    }
    if (cmd[1][0] == '#') //channl can start with '&' also
        sendToChannel(who, cmd);
    else
        sendToClient(who, cmd);
}



// two clients in same channel
// PRIVMSG #FI :hello2

// YY client will receive
// :!QQ@localhost PRIVMSG #FI :hello