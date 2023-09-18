/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yli <yli@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/16 03:09:51 by nmaliare          #+#    #+#             */
/*   Updated: 2023/09/18 20:53:36 by nmaliare         ###   ########.fr       */
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

//   :dan!~h@localhost PRIVMSG #coolpeople :Hi everyone!
//                                   ; Message from dan to the channel
//                                   #coolpeople

void    Privmsg::sendToChannel(Client& who, std::vector<std::string> cmd) const
{
    Channel*    channel = findChannel(who, cmd[1]);
    if (!channel)
    {
        who.getServer()->reply(&who,
                        "ERR_NOSUCHNICK",
                        ":No such channel");
        return ;             
    }
    std::string sub = cmd[2].substr(1);
    int size = cmd.size();
    for (int i = 3; i < size; ++i)
    {
        sub += " ";
        sub += cmd[i];
    }
	//:Sender!sender@server.com PRIVMSG #example :Hello, this is a channel message.
    channel->broadcast(who.getServer(), &who,":" + who.getNickName() + "!" +
		who.getUserName() + "@" + who.getHost() + " PRIVMSG " + cmd[1] + " :" + sub);
}

//   :Angel PRIVMSG Wiz :Hello are you receiving this message ?
//                                   ; Message from Angel to Wiz.
void    Privmsg::sendToClient(Client& who, std::vector<std::string> cmd) const
{
    Client* client = findClient(who, cmd[1]);
    if (client == NULL)
    {
        who.getServer()->reply(&who,
                        "ERR_NOSUCHNICK",
                        ":No such nick");
        return ;        
    }
    std::string sub = cmd[2].substr(1);
    int size = cmd.size();
    for (int i = 3; i < size; ++i)
    {
        sub += " ";
        sub += cmd[i];
    }
    client->getServer()->replyNoServ(client, ":" + who.getNickName() + " PRIVMSG " + client->getNickName() + " :" + sub);
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
    if (cmd[1][0] == '#' || cmd[1][0] == '&')
        sendToChannel(who, cmd);
    else
        sendToClient(who, cmd);
}