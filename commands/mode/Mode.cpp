/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yli <yli@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/16 01:37:14 by yli               #+#    #+#             */
/*   Updated: 2023/10/03 14:50:13 by nmaliare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Mode.hpp"
#include "./../../channel/Channel.hpp"
#include <stdint.h>

Mode::Mode() : Cmd() {}

Mode::~Mode() {}

Mode::Mode(const Mode& other)
{
    *this = other;
}

Mode& Mode::operator=(const Mode& other)
{
	(void)other;
    return *this;
}

void    Mode::plusmode(Client& who, std::vector<std::string> cmd, Channel*    channel) const
{
    if (cmd[2][1] == 'i' )
    {
		channel->setInviteOnly(true);
        who.getServer()->reply(&who,"RPL_CHANNELMODEIS",cmd[1] + " " + cmd[2]);
        return;
    }
    if (cmd[2][1] == 'k' || cmd[2][1] == 'l' || cmd[2][1] == 'o' || cmd[2][1] == 't')
    {
        if (cmd.size() < 4 && cmd[2][1] != 't')
        {
            who.getServer()->reply(&who,"ERR_NEEDMOREPARAMS",":Not enough parameters");
            return;
        }
        if (cmd[2][1] == 't')
        {
			channel->setTopicRight(true);
            who.getServer()->reply(&who,"RPL_CHANNELMODEIS",cmd[1] + " " + cmd[2]);
            return;
        }
        if (cmd[2][1] == 'k')
        {
            if(cmdcheck(cmd[3]))
            {
                who.getServer()->reply(&who,"ERR_UNKNOWNMODE",
									   cmd[3] + " :is unknown mode char to me");
                return;
            }
			channel->setKey(cmd[3]);
            who.getServer()->reply(&who,"RPL_CHANNELMODEIS",
                            cmd[1] + " " + cmd[2] + " " + cmd[3]);
            return;
        }
        if (cmd[2][1] == 'l' )
        {
			char *end;
			long i = std::strtol(cmd[3].c_str(), &end, 10);
            if (i < 1 || *end != '\0' || i > INT32_MAX)
            {
                who.getServer()->reply(&who,"ERR_UNKNOWNMODE",
                                cmd[3] + " :is unknown mode char to me");
                return;
            }
			channel->setUserLimit(static_cast <int>(i));
            who.getServer()->reply(&who,"RPL_CHANNELMODEIS",
                            cmd[1] + " " + cmd[2] + " " +
							(cmd[3][0] == '+' ? &(cmd[3][1]) : cmd[3]));
            return;
        }
        if (cmd[2][1] == 'o' )
        {
            Client* c = findClient(who, cmd[3]);
            if (c == NULL)
            {
                who.getServer()->reply(&who,"ERR_NOSUCHNICK",":No such nick");
                return ;
            }
			channel->addOperator(*c);
            who.getServer()->reply(&who,"RPL_CHANNELMODEIS",
                            cmd[1] + " " + cmd[2] + " " + cmd[3]);
            return;
        }
    }
    who.getServer()->reply(&who,"ERR_UNKNOWNMODE",cmd[2] + " :is unknown mode char to me");
}

void    Mode::minusmode(Client& who, std::vector<std::string> cmd, Channel*    channel) const
{
    if (cmd[2][1] == 'i' )
    {
		channel->setInviteOnly(false);
		channel->resetInvitee();
        who.getServer()->reply(&who,"RPL_CHANNELMODEIS",cmd[1] + " " + cmd[2]);
        return;
    }
    if (cmd[2][1] == 't' )
    {
		channel->setTopicRight(false);
        who.getServer()->reply(&who,"RPL_CHANNELMODEIS",cmd[1] + " " + cmd[2]);
        return;
    }
    if (cmd[2][1] == 'k' )
    {
		channel->resetKey();
        who.getServer()->reply(&who,"RPL_CHANNELMODEIS",cmd[1] + " " + cmd[2]);
        return;
    }   
    if (cmd[2][1] == 'l' )
    {
		channel->setUserLimit(INT32_MAX);
        who.getServer()->reply(&who,"RPL_CHANNELMODEIS",cmd[1] + " " + cmd[2]);
        return;
    }
    if (cmd[2][1] == 'o' )
    {
        if(cmd.size() < 4)
        {
			who.getServer()->reply(&who,"ERR_NEEDMOREPARAMS",":Not enough parameters");
            return;
        }
        Client* c = findClient(who, cmd[3]);
        if (c == NULL)
        {
            who.getServer()->reply(&who,"ERR_NOSUCHNICK",":No such nick");
            return;
        }
		channel->deleteOperator(c);
        who.getServer()->reply(&who,"RPL_CHANNELMODEIS",
							   cmd[1] + " " + cmd[2] + " " + cmd[3]);
        return;
    }
    who.getServer()->reply(&who,"ERR_UNKNOWNMODE",
						   cmd[3] + " :is unknown mode char to me");
}

void    Mode::execute(Client& who, std::vector<std::string> cmd) const
{
	if (!who.getRegistered()) {
		who.getServer()->reply(&who, "ERR_NOTREGISTERED",
							   ":You must authenticate with the server.");
		return;
	}
	if (cmd.size() < 2) {
		who.getServer()->reply(&who,
							   "ERR_NEEDMOREPARAMS",
							   ":Not enough parameters");
		return;
	}
    Channel *channel = findChannel(who, cmd[1]);
    if (!channel)
    {
        who.getServer()->reply(&who,
                                "ERR_NOSUCHCHANNEL",
							   ":No such channel");
        return;
    }
	if (cmd.size() == 2){
		std::string modes;
		modes += channel->getInviteOnly() ? "i" : "";
		modes += channel->getTopicRight() ? "t" : "";
		modes += !channel->getKey().empty() ? "k" : "";
		modes += channel->getUserLimit() < INT32_MAX ? "l" : "";
		who.getServer()->reply(&who,"RPL_CHANNELMODEIS", channel->getChannelName() + (!modes.empty() ? " +" + modes : ""));
		return;
	}
    if ((cmd[2][0] == '+' || cmd[2][0] == '-') && cmd[2][2] != 0)
    {
        who.getServer()->reply(&who,"ERR_UNKNOWNMODE",
							   cmd[3] + " :is unknown mode char to me");
        return;
    }
    if (channel->operatorRight(who))
    {
        if (cmd[2][0] == '+')
            plusmode(who, cmd, channel);
        else if (cmd[2][0] == '-')
            minusmode(who, cmd, channel);
        return;
    }
    who.getServer()->reply(&who,"ERR_NOPRIVILEGES",
                        ":Permission Denied: You're not an IRC operator");
}