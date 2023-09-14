#include "Channel.hpp"

Channel::Channel(void): _user_limit(10000), _invite_only(false), _key(NULL), _operators(NULL), _topic(NULL), _topic_right(true)
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
{
    this->_key = key;
}

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
        std::vector<Client*>::iterator it;
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
    this->_operators.pushback(&c);
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

void    Channel::addMember(Client& c)
{
    this->_Members.pushback(&c);
}

void    Channel::deleteMembers(Client& c)
{
    for (std::vector<Client*>::iterator it = this->_Members.begin(); it != this->_Members.end(); ++it)
    {
        if (*it == &c)
            this->_Members.erase(it);
        return;
    }
}

void    Channel::broadcast(std::string& msg)
{
    std::vector<Client*>::iterator it;
    for (it = this->_members.begin(); it < this->_members.end(), ++it)
        (*it)->setWriteBuff(msg);
}