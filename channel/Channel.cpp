/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user <user@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/16 02:25:34 by nmaliare          #+#    #+#             */
/*   Updated: 2023/09/16 12:57:53 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include <algorithm>

Channel::Channel(void): _user_limit(10000), _invite_only(false), _topic_right(true)
{
}

Channel::~Channel() {}

Channel::Channel(const Channel& other)
{
    *this = other;
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

void    Channel::setUserLimit(size_t user_limit)
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
    if (this->_operators.empty())
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

void    Channel::addOperator(Client& c)
{
    this->_operators.push_back(&c);
}

void    Channel::deleteOperator(Client& c)
{
    for (std::vector<Client*>::iterator it = this->_operators.begin(); it != this->_operators.end(); ++it)
    {
        if (*it == &c)
            this->_operators.erase(it);
        return;
    }
}
void    Channel::deleteOperator(Client* c)
{
	std::cout << BLUE"deleting operator "RES << c->getNickName() << std::endl;
	this->_operators.erase(std::find(this->_operators.begin(), this->_operators.end(), c));
}
void    Channel::addMember(Client& c)
{
	if (this->_members.size() < 1)
		this->_operators.push_back(&c);
    this->_members.push_back(&c);
}

void    Channel::deleteMembers(Client* c)
{
	std::cout << BLUE"deleting member "RES << c->getNickName() << std::endl;
	this->_members.erase(std::find(this->_members.begin(), this->_members.end(), c));
}

//this one didnt delete it >.<, we have to delete by pointer, not reference.
// the one up works :)
void    Channel::deleteMembers(Client& c)
{
    for (std::vector<Client*>::iterator it = this->_members.begin(); it != this->_members.end(); ++it)
    {
        if (*it == &c)
            this->_members.erase(it);
        return;
    }
}

std::vector<Client *> &Channel::getMembers()
{
	return this->_members;
}

void    Channel::broadcast(Server * server, std::string& msg)
{
    server->reply(this->_members, "", msg);
}

std::vector<Client*> &Channel::getOperators(void)
{
    return this->_operators;
}