#include "Channel.hpp"

Channel::Channel(void): _user_limit(10000), _invite_only(false), _key("")
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

bool    Channel::getInviteOnly(void)
{
    return this->_invite_only;
}