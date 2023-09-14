#include "Mode.hpp"

Mode::Mode() : Cmd(){}

Mode::~Mode() {}

Mode::Mode(const Mode& other)
{
    *this = other;
}

Mode& Mode::operator=(const Mode& other)
{
    return *this;
}

Channel* Mode::findChannel(Client& who, std::string channelName)
{
    std::map<std::string, Channel *>::iterator it;
    std::map<std::string, Channel *> channels = who.getServer()->getChannels();
    for(it = channels.begin(); it != channels.end(); ++it)
    {
        if (it->first == channelName)
            return *(it->second);
    }
    return NULL;
}

void    Mode::setInviteOnly(void)
{
    this->_channel->setInviteOnly(true);
}

void    Mode::unsetsetInviteOnly(void)
{
    this->_channel->setInviteOnly(false);
}

void    Mode::setKey(const std::string key)
{
    this->_channel->setKey(key);
}

void    Mode::unsetKey(void)
{
    this->_channel->setKey(NULL);
}

void    Mode::setTopicRgiht(void)
{
    this->_channel->setTopicRight(true);
}

void    Mode::unsetsetTopicRight(void)
{
    this->_channel->setTopicRight(false);
}

void    Mode::setUserLimit(size_t size)
{
    this->_channel->setUserLimit(size);
}

void    Mode::unsetUserLimit(void)
{
    this->_channel->setUserLimit(100000);
}

Client& Mode::findClient(Client& who, std::string nickName)
{
    std::vector<Client*>::iterator it;
    for(it = who.getServer().getClient().begin(); it != who.getServer().getClient().end(); ++it)
    {
        if (*it)->getNickName() == nickName;
        return it;
    }
    return NULL;
}
void    setUserPrivilege(Client& c)
{
    this->_channel->addOperator(c);
}

void    unsetUserPrivileg(Client& c)
{
    this->_channel->deleteOperator(c);
}

bool    Mode::thirdcmdcheck(std::string cmd)
{
    for(unsigned long i = 0; i < cmd.size(); ++i)
    {
        if (cmd[i] == '#' || cmd[i] == '&' || cmd[i] == '+' || cmd[i] == '@' 
            || cmd[i] == '%' || cmd[i] == '/' || cmd[i] == '*' || cmd[i] == '?' ||
            cmd[i] > 127 || cmd[i] <= 32)
        return true;
    }
    return false;
}

void    Mode::plusmode(Client& who, std::vector<std::string> cmd)
{
    if (cmd[2][1] == 'i' )
        setInviteOnly();
    if (cmd[2][1] == 'k' || cmd[2][1] == 'l' || cmd[2][1] == 'o')
    {
        if (cmd[3] == NULL)
        {
            who.getServer()->reply(&who,
                                "ERR_NEEDMOREPARAMS",
                                ":Not enough parameters");
            return;            
        }
    }
    if (cmd[2][1] == 't' )
    {
        if(thirdcmdcheck(cmd[3]))
        {
            who.getServer()->reply(&who,
                                "ERR_UNKNOWNMODE",
                            ":is unknown mode char to me");
            return;
        }     
        setTopicRight();
        return;
    }
    if (cmd[2][1] == 'k' )
    {
        if(thirdcmdcheck(cmd[3]))
        {
            who.getServer()->reply(&who,
                                "ERR_UNKNOWNMODE",
                            ":is unknown mode char to me");
            return;
        }        
        setKey(cmd[3]);
        return;
    }
    if (cmd[2][1] == 'l' )
    {
        int i = std::stoi(cmd[3]);
        if (i <= 1)
        {
            who.getServer()->reply(&who,
                                "ERR_UNKNOWNMODE",
							   ":is unknown mode char to me");
            return;          
        }
        setUserLimit((size_t)i);
        return;
    }
    if (cmd[2][1] == 'o' )
    {
        Client* c = findClient(cmd[3])
        if (!c)
        {
            who.getServer()->reply(&who,
                            "ERR_NOSUCHNICK",
                            ":No such nick/channel");
            return ;    
        }
        setUserPrivilege(c);
        return;
    } 
    who.getServer()->reply(&who,
                        "ERR_UNKNOWNMODE",
                    ":is unknown mode char to me");
    return;
}

void    Mode::minusmode(Client& who, std::vector<std::string> cmd)
{
    if (cmd[2][1] == 'i' )
    {
        unsetsetInviteOnly();
        return;
    }
    if (cmd[2][1] == 't' )
    {
        unsetTopicRight();
        return;
    }
    if (cmd[2][1] == 'k' )
    {
        unsetKey();
        return;
    }   
    if (cmd[2][1] == 'l' )
    {
        unsetsetUserLimit();
        return;
    }
    if (cmd[2][1] == 'o' )
    {
        if(cmd[3] == NULL || thirdcmdcheck(cmd[3]))
        {
            who.getServer()->reply(&who,
                                "ERR_UNKNOWNMODE",
                            ":is unknown mode char to me");
            return;
        }
        Client* c = findClient(cmd[3])
        if (!c)
        {
            who.getServer()->reply(&who,
                            "ERR_NOSUCHNICK",
                            ":No such nick/channel");
            return;    
        }
        unsetUserPrivilege(c);
        return;
    } 
    who.getServer()->reply(&who,
                        "ERR_UNKNOWNMODE",
                    ":is unknown mode char to me");
}

void    Mode::execute(Client& who, std::vector<std::string> cmd) const
{
	if (who.getAuthenticated()){
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
    Channel *c = findChannel(who, cmd[1]);
    if (!c)
    {
        who.getServer()->reply(&who,
                                "ERR_NOSUCHCHANNEL",
							   ":No such channel");
        return;   
    }
    this->_channel = c;
    if (cmd[2][0] != '+' || cmd[2][0] != '-' || cmd[2][2] != 0)
    {
        who.getServer()->reply(&who,
                                "ERR_UNKNOWNMODE",
							   ":is unknown mode char to me");
        return;          
    }
    if (this->_channel->operatorRight(who))
    {
        if (cmd[2][0] == '+')
            plusmode(who, cmd);
        else if (cmd[2][0] == '-')
            minusmode(who, cmd);
        return;
    }
    // if (!this->_channel->operatorRight(who))
    // {
    //     if (cmd[2][1] == 't')
    //     {
    //         if (cmd[3] == NULL)
    //         {
    //             who.getServer()->reply(&who,
    //                                 "ERR_NEEDMOREPARAMS",
    //                                 ":Not enough parameters");          
    //         }
    //         if ((cmd[2][0] == '+'))
    //         {
    //             if(thirdcmdcheck(cmd[3]))
    //             {
    //                 who.getServer()->reply(&who,
    //                                     "ERR_UNKNOWNMODE",
    //                                 ":is unknown mode char to me");
    //             }     
    //             setTopic(cmd[3]);
    //         }
    //         if ((cmd[2][0] == '-'))
    //             unsetTopic();
    //         return;
    //     }     
    // }
    who.getServer()->reply(&who,
                        "ERR_NOPRIVILEGES",
                        ":Permission Denied- You're not an IRC operator");
}