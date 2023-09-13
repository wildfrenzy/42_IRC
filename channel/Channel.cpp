#include "Channel.hpp"

Channel::Channel(void): _user_limit(10000), _invite_only(false), _key(NULL), _operators(NULL), _key(NULL), _topic(NULL)
{
}

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
[
    this->_key = key;
]

void    Channel::setTopic(const std::string topic)
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
    if (this->_operator == NULL)
        return true;
    else
    {
        std::set<Client*>::iterator it;
        for(it = this->_operators.begin(); it != this->_operators.end(); ++it)
        {
            if ((*it).getNickName() == c.getNickName())
                return true;
        }
    }
    return false;
}

size_t  Channel::getMemberSize(void)
{
    return this->_members.size();
}