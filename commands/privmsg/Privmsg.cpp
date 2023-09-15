#include "Privmsg.hpp"

Privmsg::Privmsg(void): /*Cmd(), */Mode(){}

Privmsg::~Privmsg(void) {}

Privmsg::Privmsg(const Privmsg& other)
{
    *this = other;
}

Privmsg& Privmsg::operator=(const Privmsg& other)
{
    return *this;
}

void    Privmsg::sendToChannel(Client& who, std::vector<std::string> cmd) const
{
    Channel*    channel = findChannel(who, cmd[1]);
    if (!channel)
    {
        who.getServer()->reply(&who,
                        "ERR_NOSUCHNICK",
                        ":No such nick/channel");
        return ;             
    }
    std::vector<std::string>::iterator it;
    for (it = cmd.begin() + 2; it < cmd.end(); ++it)
        channel->broadcast(who.getServer(), *it);
}

void    Privmsg::sendToClient(Client& who, std::vector<std::string> cmd) const
{
    Client& client = findClient(who, cmd[1]);
    if (client.getNickName() == who.getServer()->getClients().front()->getNickName())
    {
        who.getServer()->reply(&who,
                        "ERR_NOSUCHNICK",
                        ":No such nick/channel");
        return ;        
    }
    std::vector<std::string>::iterator it;
    for (it = cmd.begin() + 2; it < cmd.end(); ++it)
        client.setWriteBuff((*it));
}


void    Privmsg::execute(Client& who, std::vector<std::string> cmd) const
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
    if (cmd[1][0] == '#')
        sendToChannel(who, cmd);
    else
        sendToClient(who, cmd);
}



// two clients in same channel
// PRIVMSG #FI :hello2

// YY client will receive
// :!QQ@localhost PRIVMSG #FI :hello