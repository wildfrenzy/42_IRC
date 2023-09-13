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
    this->_channel->setInviteOnly() = true;
}

void    Mode::unsetsetInviteOnly(void)
{
    this->_channel->setInviteOnly() = false;
}

void    Mode::setKey(const std::string key)
{
    this->_channel->setKey(key);
}

void    Mode::unsetKey(void)
{
    this->_channel->setKey(NULL);
}

void    Mode::setTopic(const std::string topic)
{
    this->_channel->setTopic(topic);
}

void    Mode::unsetsetTopic(void)
{
    this->_channel->setTopic(NULL);
}

void    Mode::setUserLimit(size_t size)
{
    this->_channel->setUserLimit(size);
}

void    Mode::unsetUserLimit(void)
{
    this->_channel->setUserLimit(100000);
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
    if (cmd[2][1] == 't' || cmd[2][1] == 'k' || cmd[2][1] == 'l')
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
        setTopic(cmd[3]);
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
    who.getServer()->reply(&who,
                        "ERR_UNKNOWNMODE",
                    ":is unknown mode char to me");
    return;
}

void    Mode::minusmode(Client& whostd::vector<std::string> cmd)
{
    if (cmd[2][1] == 'i' )
    {
        unsetsetInviteOnly();
        return;
    }
    if (cmd[2][1] == 't' )
    {
        unsetTopic();
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
    Channel *c = NULL;
    c = findChannel(who, cmd[1]);
    if (c)
        this->_channel = c;
    else
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
    if (this->_channel->operatorRight(who))
    {
        if (cmd[2][0] == '+')
            plusmode(who, cmd);
        else if (cmd[2][0] == '-')
            minusmode(who, cmd);
        return;
    }
    if (!this->_channel->operatorRight(who))
    {
        if (cmd[2][1] == 't')
        {
            if (cmd[3] == NULL)
            {
                who.getServer()->reply(&who,
                                    "ERR_NEEDMOREPARAMS",
                                    ":Not enough parameters");
                return;            
            }
            if ((cmd[2][0] == '+'))
            {
                if(thirdcmdcheck(cmd[3]))
                {
                    who.getServer()->reply(&who,
                                        "ERR_UNKNOWNMODE",
                                    ":is unknown mode char to me");
                    return;
                }     
                setTopic(cmd[3]);
            }
            if ((cmd[2][0] == '-'))
                unsetTopic();
            return;
        }     
    }
    who.getServer()->reply(&who,
                        "ERR_NOPRIVILEGES",
                        ":Permission Denied- You're not an IRC operator");
}