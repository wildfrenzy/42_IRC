#include "Mode.hpp"
#include "./../../channel/Channel.hpp"

Mode::Mode() : Cmd() {}

Mode::~Mode() {}

Mode::Mode(const Mode& other)
{
    *this = other;
}

Mode& Mode::operator=(const Mode& other)
{
    return *this;
}

// Channel* Mode::findChannel(Client& who, std::string channelName) const
// {
//     std::map<std::string, Channel *>::iterator it;
//     std::map<std::string, Channel *> channels = who.getServer()->getChannels();
//     for(it = channels.begin(); it != channels.end(); ++it)
//     {
//         if (it->first == channelName)
//             return it->second;
//     }
//     return NULL;
// }

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
    channel->setKey(NULL);
}

void    Mode::setTopicRight(Channel*    channel) const
{
    channel->setTopicRight(true);
}

void    Mode::unsetTopicRight(Channel*    channel) const
{
    channel->setTopicRight(false);
}

void    Mode::setUserLimit(size_t size, Channel*    channel) const
{
    channel->setUserLimit(size);
}

void    Mode::unsetUserLimit(Channel*    channel) const
{
    channel->setUserLimit(100000);
}

// Client& Mode::findClient(Client& who, std::string nickName) const
// {
//     std::vector<Client*>::const_iterator it;

//     for(it = who.getServer()->getClients().begin(); it != who.getServer()->getClients().end(); ++it)
//     {
//         if ((*it)->getNickName() == nickName)
//             return *(*it);
//     }
//     return *who.getServer()->getClients().front(); //set bot as first client !!!!
// }

void    Mode::setUserPrivilege(Client& c, Channel*    channel) const
{
    channel->addOperator(c);
}

void    Mode::unsetUserPrivilege(Client& c, Channel*    channel) const
{
    channel->deleteOperator(c);
}

// bool    Mode::thirdcmdcheck(std::string cmd) const
// {
//     for(unsigned long i = 0; i < cmd.size(); ++i)
//     {
//         if (cmd[i] == '#' || cmd[i] == '&' || cmd[i] == '+' || cmd[i] == '@' 
//             || cmd[i] == '%' || cmd[i] == '/' || cmd[i] == '*' || cmd[i] == '?' ||
//             cmd[i] > 127 || cmd[i] <= 32)
//         return true;
//     }
//     return false;
// }

void    Mode::plusmode(Client& who, std::vector<std::string> cmd, Channel*    channel) const
{
    if (cmd[2][1] == 'i' )
        setInviteOnly(channel);
    if (cmd[2][1] == 'k' || cmd[2][1] == 'l' || cmd[2][1] == 'o')
    {
        if (cmd[3].empty())
        {
            who.getServer()->reply(&who,
                                "ERR_NEEDMOREPARAMS",
                                ":Not enough parameters");
            return;            
        }
        if (cmd[2][1] == 't' )
        {
            if(cmdcheck(cmd[3]))
            {
                who.getServer()->reply(&who,
                                    "ERR_UNKNOWNMODE",
                                ":is unknown mode char to me");
                return;
            }     
            setTopicRight(channel);
            return;
        }
        if (cmd[2][1] == 'k' )
        {
            if(cmdcheck(cmd[3]))
            {
                who.getServer()->reply(&who,
                                    "ERR_UNKNOWNMODE",
                                ":is unknown mode char to me");
                return;
            }        
            setKey(cmd[3], channel);
            return;
        }
        if (cmd[2][1] == 'l' )
        {
            int i = atoi(cmd[3].c_str());
            if (i <= 1)
            {
                who.getServer()->reply(&who,
                                    "ERR_UNKNOWNMODE",
                                ":is unknown mode char to me");
                return;          
            }
            setUserLimit((size_t)i, channel);
            return;
        }
        if (cmd[2][1] == 'o' )
        {
            Client& c = findClient(who, cmd[3]);
            if (c.getNickName() == who.getServer()->getClients().front()->getNickName())
            {
                who.getServer()->reply(&who,
                                "ERR_NOSUCHNICK",
                                ":No such nick/channel");
                return ;    
            }
            setUserPrivilege(c, channel);
            return;
        }
    }
    who.getServer()->reply(&who,
                        "ERR_UNKNOWNMODE",
                    ":is unknown mode char to me");
    return;
}

void    Mode::minusmode(Client& who, std::vector<std::string> cmd, Channel*    channel) const
{
    if (cmd[2][1] == 'i' )
    {
        unsetInviteOnly(channel);
        return;
    }
    if (cmd[2][1] == 't' )
    {
        unsetTopicRight(channel);
        return;
    }
    if (cmd[2][1] == 'k' )
    {
        unsetKey(channel);
        return;
    }   
    if (cmd[2][1] == 'l' )
    {
        unsetUserLimit(channel);
        return;
    }
    if (cmd[2][1] == 'o' )
    {
        if(cmd[3].empty() || cmdcheck(cmd[3]))
        {
            who.getServer()->reply(&who,
                                "ERR_UNKNOWNMODE",
                            ":is unknown mode char to me");
            return;
        }
        Client& c = findClient(who, cmd[3]);
        if (c.getNickName() == who.getServer()->getClients().front()->getNickName())
        {
            who.getServer()->reply(&who,
                            "ERR_NOSUCHNICK",
                            ":No such nick/channel");
            return;    
        }
        unsetUserPrivilege(c, channel);
        return;
    } 
    who.getServer()->reply(&who,
                        "ERR_UNKNOWNMODE",
                    ":is unknown mode char to me");
}

void    Mode::execute(Client& who, std::vector<std::string> cmd) const
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
    Channel *channel = findChannel(who, cmd[1]);
    if (!channel)
    {
        who.getServer()->reply(&who,
                                "ERR_NOSUCHCHANNEL",
							   ":No such channel");
        return;   
    }
    if (cmd[2][0] != '+' || cmd[2][0] != '-' || cmd[2][2] != 0)
    {
        who.getServer()->reply(&who,
                                "ERR_UNKNOWNMODE",
							   ":is unknown mode char to me");
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
                        ":Permission Denied- You're not an IRC operator");
}