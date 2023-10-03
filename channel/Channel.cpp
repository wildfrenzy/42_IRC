/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yli <yli@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/16 02:25:34 by nmaliare          #+#    #+#             */
/*   Updated: 2023/10/03 00:26:59 by nmaliare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include <algorithm>
#include <stdint.h>

Channel::Channel(): _user_limit(INT32_MAX), _invite_only(false), _topic_right(true)
{}

Channel::~Channel() {}

Channel::Channel(const Channel& other)
{
	(void)other;
}

Channel& Channel::operator=(const Channel& other)
{
    this->_members = other._members;
    this->_operators = other._operators;
    return *this;
}

void    Channel::setInviteOnly(bool invite_only)
{
    this->_invite_only = invite_only;
}

void    Channel::setUserLimit(int user_limit)
{
    this->_user_limit = user_limit;
}

size_t  Channel::getUserLimit(void)
{
    return this->_user_limit;
}

std::string Channel::getKey(void)
{
    return this->_key;
}

void    Channel::setKey(const std::string key)
{
    this->_key = key;
}

void	Channel::resetKey() {
	this->_key.erase();
}

void    Channel::setTopic(std::string& topic)
{
    this->_topic = topic;
}

bool    Channel::getInviteOnly(void)
{
    return this->_invite_only;
}

void    Channel::setChannelName(std::string& channelName)
{
    this->_channelName = channelName;
}

std::string Channel::getChannelName(void)
{
    return this->_channelName;
}

bool    Channel::operatorRight(Client& c)
{
    if (this->_operators.empty()) //?
        return true;
    else
    {
        std::vector<Client*>::iterator it;
        for(it = this->_operators.begin(); it != this->_operators.end(); ++it)
        {
            if ((*it)->getNickName() == c.getNickName())
                return true;
        }
    }
    return false;
}

size_t  Channel::getMemberSize(void)
{
    return this->_members.size();
}

void    Channel::setTopicRight(bool topic_right)
{
    this->_topic_right = topic_right;
}

bool    Channel::getTopicRight(void)
{
    return this->_topic_right;
}

std::string Channel::getTopic(void)
{
    return this->_topic;
}

void    Channel::unsetTopic(void)
{
    this->_topic = "";
}

void    Channel::addOperator(Client& c)
{
    this->_operators.push_back(&c);
}

void    Channel::deleteOperator(Client* c)
{
	std::vector <Client *>::iterator i = std::find(this->_operators.begin(),
													 this->_operators.end(), c);
	if (i != this->_operators.end()){
		std::cout << BLUE"deleting " + _channelName + " operator "RES << c->getNickName() << std::endl;
		this->_operators.erase(i);
		if (this->_operators.empty() && !this->_members.empty())
		{
			Client* tmp = this->_members[0];
			this->_operators.push_back(tmp);
		}
	}
}
bool    Channel::belongToGroup(Client& who, std::vector<Client*> group)
{
    for (std::vector<Client*>::iterator it = group.begin(); it != group.end(); ++it)
    {
        if ((*it)->getNickName() == who.getNickName())
            return true;
    }
    return false;
}

void    Channel::addMember(Client& c)
{
	if (this->_members.empty())
		this->_operators.push_back(&c);
    this->_members.push_back(&c);
}

void    Channel::addInvitee(Client* c)
{
	std::vector <Client *>::iterator i = std::find(this->_invitee.begin(), this->_invitee.end(), c);
	if (i == this->_invitee.end())
		this->_invitee.push_back(c);
}

std::vector<Client*> &Channel::getInvitee()
{
    return this->_invitee;
}

void    Channel::deleteMembers(Client* c)
{
	std::vector <Client *>::iterator i = std::find(this->_members.begin(), this->_members.end(), c);
	if (i != this->_members.end()){
		std::cout << BLUE"deleting "  + _channelName + " member "RES << c->getNickName() << std::endl;
		this->_members.erase(i);
	}
}

void	Channel::removeInvitee(Client* c)
{
	std::vector <Client *>::iterator i = std::find(this->_invitee.begin(), this->_invitee.end(), c);
	if (i != this->_invitee.end())
		this->_invitee.erase(i);
}

void	Channel::resetInvitee()
{
	this->_invitee.clear();
}

std::vector<Client *> &Channel::getMembers()
{
	return this->_members;
}

void	Channel::broadcast(Server* server, Client* c, std::string msg)
{
	std::vector <Client *> broadcast = this->_members;
	std::vector<Client*>::iterator it = std::find(broadcast.begin(), broadcast.end(), c);
	broadcast.erase(it);
	server->replyNoServ(broadcast, msg);
}

std::vector<Client*> &Channel::getOperators(void)
{
    return this->_operators;
}