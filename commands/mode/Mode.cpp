/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yli <yli@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/16 01:37:14 by yli               #+#    #+#             */
/*   Updated: 2023/10/03 13:44:18 by nmaliare         ###   ########.fr       */
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

void    Mode::setInviteOnly(Channel*    channel) const
{
    channel->setInviteOnly(true);
}

void    Mode::unsetInviteOnly(Channel*    channel) const
{
    channel->setInviteOnly(false);
}

void    Mode::setKey(const std::string key, Channel*    channel) const
{
    channel->setKey(key);
}

void    Mode::unsetKey(Channel*    channel) const
{
    channel->setKey("");
}

void    Mode::setTopicRight(Channel*    channel) const
{
    channel->setTopicRight(true);
}

void    Mode::unsetTopicRight(Channel*    channel) const
{
    channel->setTopicRight(false);
}

void    Mode::unsetUserLimit(Channel*    channel) const
{
    channel->setUserLimit(100000);
}

void    Mode::setUserPrivilege(Client& c, Channel*    channel) const
{
    channel->addOperator(c);
}

void    Mode::plusmode(Client& who, std::vector<std::string> cmd, Channel*    channel) const
{
    if (cmd[2][1] == 'i' )
    {
        setInviteOnly(channel);
        who.getServer()->reply(&who,
                            "RPL_CHANNELMODEIS",
                            cmd[1] + " " + cmd[2]);            
        return; 
    }
    if (cmd[2][1] == 'k' || cmd[2][1] == 'l' || cmd[2][1] == 'o' || cmd[2][1] == 't')
    {
        if (cmd[3].empty() && (cmd[2][1] == 'k' || cmd[2][1] == 'l' || cmd[2][1] == 'o'))
        {
            who.getServer()->reply(&who,
                                "ERR_NEEDMOREPARAMS",
                                ":Not enough parameters");
            return;            
        }
        if (cmd[2][1] == 't')
        {   
            setTopicRight(channel);
            who.getServer()->reply(&who,
                                "RPL_CHANNELMODEIS",
                                cmd[1] + " " + cmd[2]);            
            return;
        }
        if (cmd[2][1] == 'k' )
        {
            if(cmdcheck(cmd[3]))
            {
                who.getServer()->reply(&who,
                                    "ERR_UNKNOWNMODE",
                                cmd[3] + " :is unknown mode char to me");
                return;
            }   
            setKey(cmd[3], channel);
            who.getServer()->reply(&who,
                                "RPL_CHANNELMODEIS",
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
            who.getServer()->reply(&who,
                                "RPL_CHANNELMODEIS",
                            cmd[1] + " " + cmd[2] + " " +
							(cmd[3][0] == '+' ? &(cmd[3][1]) : cmd[3]));
            return;
        }
        if (cmd[2][1] == 'o' )
        {
            Client* c = findClient(who, cmd[3]);
            if (c == NULL)
            {
                who.getServer()->reply(&who,
                                "ERR_NOSUCHNICK",
                                ":No such nick/channel");
                return ;    
            }
            setUserPrivilege(*c, channel);
            who.getServer()->reply(&who,
                                "RPL_CHANNELMODEIS",
                            cmd[1] + " " + cmd[2] + " " + cmd[3]);
            return;
        }
    }
    who.getServer()->reply(&who,
                        "ERR_UNKNOWNMODE",
                    cmd[2] + " :is unknown mode char to me");
    return;
}

void    Mode::minusmode(Client& who, std::vector<std::string> cmd, Channel*    channel) const
{
    if (cmd[2][1] == 'i' )
    {
        unsetInviteOnly(channel);
        who.getServer()->reply(&who,
                            "RPL_CHANNELMODEIS",
                            cmd[1] + " " + cmd[2]);   
        return;
    }
    if (cmd[2][1] == 't' )
    {
        unsetTopicRight(channel);
        who.getServer()->reply(&who,
                            "RPL_CHANNELMODEIS",
                        cmd[1] + " " + cmd[2]);        
        return;
    }
    if (cmd[2][1] == 'k' )
    {
        unsetKey(channel);
        who.getServer()->reply(&who,
                            "RPL_CHANNELMODEIS",
                        cmd[1] + " " + cmd[2]);  
        return;
    }   
    if (cmd[2][1] == 'l' )
    {
        unsetUserLimit(channel);
        who.getServer()->reply(&who,
                            "RPL_CHANNELMODEIS",
                        cmd[1] + " " + cmd[2]);  
        return;
    }
    if (cmd[2][1] == 'o' )
    {
        if(cmd[3].empty())
        {
            who.getServer()->reply(&who,
                                "ERR_UNKNOWNMODE",
                            ":is unknown mode char to me");
            return;
        }
        Client* c = findClient(who, cmd[3]);
        if (c == NULL)
        {
            who.getServer()->reply(&who,
                            "ERR_NOSUCHNICK",
                            ":No such nick");
            return;    
        }
		channel->deleteOperator(c);
        who.getServer()->reply(&who,
                            "RPL_CHANNELMODEIS",
                        cmd[1] + " " + cmd[2] + " " + cmd[3]);  
        return;
    } 
    who.getServer()->reply(&who,
                        "ERR_UNKNOWNMODE",
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
		modes += channel->getUserLimit() < 10000 ? "l" : "";
		who.getServer()->reply(&who,"RPL_CHANNELMODEIS", channel->getChannelName() + (!modes.empty() ? " +" + modes : ""));
		return;
	}
    if ((cmd[2][0] == '+' || cmd[2][0] == '-') && cmd[2][2] != 0)
    {
        who.getServer()->reply(&who,
                                "ERR_UNKNOWNMODE",
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
    who.getServer()->reply(&who,
                        "ERR_NOPRIVILEGES",
                        ":Permission Denied: You're not an IRC operator");
}