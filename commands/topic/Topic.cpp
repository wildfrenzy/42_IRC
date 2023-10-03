/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yli <yli@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 18:53:35 by yli               #+#    #+#             */
/*   Updated: 2023/10/02 22:47:18 by nmaliare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Topic.hpp"
#include "./../../client/Client.hpp"

Topic::Topic() :Cmd(){}

Topic::~Topic() {}

Topic::Topic(const Topic& other)
{
    *this = other;
}

Topic&  Topic::operator=(const Topic& other)
{
	(void)other;
    return *this;
}

void    Topic::setTopic(std::string topic, Channel *c) const
{
    c->setTopic(topic);
}

bool    Topic::checkOperatorRight(Client& who, Channel *c) const
{
    std::vector<Client*>::iterator it;
    for (it = c->getOperators().begin(); it != c->getOperators().end(); ++it)
    {
        if ((*it)->getNickName() == who.getNickName())
            return true;
    }
    return false;
}

void    Topic::unsetTopic(Channel *c) const
{
    c->unsetTopic();
}

void    Topic::checkTopic(Client& who, Channel *c, std::string channelName) const
{
    if (c->getTopic().empty())
		who.getServer()->reply(&who, "RPL_NOTOPIC", channelName + " :No topic is set");
    else
        who.getServer()->reply(&who,
                        "RPL_TOPIC",
                        " " + channelName + " :" + c->getTopic());
}

void    Topic::execute(Client& who, std::vector<std::string> cmd) const
{
	if (!who.getRegistered()) {
		who.getServer()->reply(&who, "ERR_NOTREGISTERED",
							   ":You must authenticate with the server.");
		return;
	}
	if (cmd.size() < 1) {
		who.getServer()->reply(&who,
							   "ERR_NEEDMOREPARAMS",
							   ":Not enough parameters");
		return;
	}
    Channel *c = NULL;
    c = findChannel(who, cmd[1]);
    if(!c)
    {
        who.getServer()->reply(&who,
                                "ERR_NOSUCHCHANNEL",
							   ":No such channel");
        return;   
    }
    if (!c->belongToGroup(who, c->getMembers()))
    {
        who.getServer()->reply(&who,
                                "ERR_NOTONCHANNEL",
							   cmd[1] + " :You're not on that channel");
        return; 
    }
    if (cmd.size() == 2)
    {
        checkTopic(who, c, cmd[1]);
        return;
    }
    if (c->getTopicRight() && !checkOperatorRight(who, c))
    {
        who.getServer()->reply(&who,
                        "ERR_CHANOPRIVSNEEDED",
                        c->getChannelName() + " :You're not channel operator");
        return;
    }
    if(cmdcheck(cmd[2]) || cmd[2][0] != ':')
    {
        who.getServer()->reply(&who,
                            "ERR_UNKNOWNERROR",
                            ":invalid topic");
        return;
    }
    if (cmd[2].size() != 1)
    {
        std::string sub = cmd[2].substr(1);
        int size = cmd.size();
        for (int i = 3; i < size; ++i)
        {
            sub += " ";
            sub += cmd[i];
        }
		setTopic(sub, c);
		who.getServer()->reply(c->getMembers(),"RPL_TOPIC", cmd[1] + " :" + sub);
		who.getServer()->replyTime(&who, c->getMembers(),"RPL_TOPICWHOTIME",cmd[1]);
    }
    else
    {
        unsetTopic(c);
        who.getServer()->reply(c->getMembers(),
                            "RPL_TOPIC", cmd[1] + " " + cmd[2]);
    }
}