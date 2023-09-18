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

void    Topic::setTopic(std::string& topic, Channel *c) const
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
    if (c->getTopic() == "")
        who.getServer()->reply(&who,
                        "RPL_TOPIC",
                        " " + channelName + " :no topic");
    else
        who.getServer()->reply(&who,
                        "RPL_TOPIC",
                        " " + channelName + " :" + c->getTopic());
}

void    Topic::execute(Client& who, std::vector<std::string> cmd) const
{
	if (!who.getAuthenticated()){
		who.getServer()->reply(&who,
										 "ERR_ALREADYREGISTERED",
										 ":You may not reregister");
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
    if (cmd.size() == 2)
    {
        checkTopic(who, c, cmd[1]);
        return;
    }
    if (!c->getTopicRight() || !checkOperatorRight(who, c))
    {
        who.getServer()->reply(&who,
                        "ERR_CHANOPRIVSNEEDED",
                        ":You're not channel operator");        
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
        who.getServer()->replyTime(&who, c->getMembers(),"RPL_TOPICWHOTIME",
                            cmd[1]);
    }
    else
    {
        unsetTopic(c);
		//change this reply
        who.getServer()->reply(&who,
                            "RPL_TOPIC",
                            " " + cmd[1] + " :topic unset");
    }
}

//RPL_TOPIC (332) 
//  "<client> <channel> :<topic>"
//Sent to a client when joining the <channel> to inform them of the current topic of the channel.


//RPL_TOPICWHOTIME (333) 
//  "<client> <channel> <nick> <setat>"
//Sent to a client to let them know who set the topic (<nick>) and when they set it (<setat> is a unix timestamp). Sent after RPL_TOPIC 

/*
ERR_CHANOPRIVSNEEDED (482) 
  "<client> <channel> :You're not channel operator"
Indicates that a command failed because the client does not have the appropriate channel privileges. 
This numeric can apply for different prefixes such as halfop, operator, etc. 
The text used in the last param of this message may vary.
*/