#include "Topic.hpp"

Topic::Topic() : Cmd(), Mode(){}

Topic::~Topic() {}

Topic::Topic(const Topic& other)
{
    *this = other;
}

Topic&  Topic::operator=(const Topic& other)
{
    return *this;
}

void    Topic::setTopic(const std::string topic)
{
    this->_channel->setTopic(topic);
}

bool    Topic::checkOperatorRight(Client& who)
{
    std::vector<Client*>::iterator it;
    for (it = this->_c->_operators.begin(); it != this->_c->_operators.end(); ++it)
    {
        if (*it)->getNickName() == who.getNickName()
            return true;
    }
    return false
}
void    Topic::execute(Client& who, std::vector<std::string> cmd) const
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
    if (!this->_channel->getTopicRight() || !checkOperatorRight(who))
    {
        who.getServer()->reply(&who,
                        "ERR_CHANOPRIVSNEEDED",
                        ":You're not channel operator");        
        return;
    }
    if(thirdcmdcheck(cmd[2]))
    {
        // who.getServer()->reply(&who,
        //                     "ERR_NEEDMOREPARAMS",
        //                     ":Not enough parameters");     //can not find this error type   
        return;
    }
    setTopic(cmd[2]);
    std::time_t currentTime;
    std::time(&currentTime);
    who.getServer()->reply(&who,
                        this->_c->getChannelName(),
                        &who.getNickName(),
                        currentTime); 
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