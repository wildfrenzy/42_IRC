#ifndef MODE_HPP
#define MODE_HPP

#include "./../../cmd/Cmd.hpp"
#include "./../../channel/Channel.hpp"
#include "./../../client/Client.hpp"

class Client;
class Cmd;
class Channel;
class Server;


class   Mode: public Cmd
{
    public:
        Mode(void);
        ~Mode(void);
        Mode(const Mode& other);
        Mode& operator=(const Mode& other);
    
        virtual    void    execute(Client& who, std::vector<std::string> cmd) const;
        Channel* findChannel(Client& who, std::string channelName) const;
        Client& findClient(Client& who, std::string nickName) const;

        void    setInviteOnly(Channel*    channel) const;
        void    unsetInviteOnly(Channel*    channel) const;
        void    setKey(const std::string key, Channel*    channel) const;
        void    unsetKey(Channel*    channel) const;
        void    setTopicRight(Channel*    channel) const;
        void    unsetTopicRight(Channel*    channel) const;
        void    setUserLimit(size_t size, Channel*    channel) const;
        void    unsetUserLimit(Channel*    channel) const;
        void    setUserPrivilege(Client& c, Channel*    channel) const;
        void    unsetUserPrivilege(Client& c, Channel*    channel)  const;
        void    plusmode(Client& who, std::vector<std::string> cmd, Channel*    channel) const;
        void    minusmode(Client& who, std::vector<std::string> cmd, Channel*    channel) const;
        bool    thirdcmdcheck(std::string content) const;
};


#endif

/*
Setting a channel mode:
/MODE #channel +/-mode parameters


To set the channel as invite-only:
/MODE #example +i

TO set the Channel Key:
/MODE #example +k mysecretpassword

To set the channel topic (used by channel operators):
/MODE #example +t 

To give/take channel operator privilege
/MODE #channel +o Nickname

/MODE #channel +l limit
#channel: Replace this with the name of the channel where you want to set the user limit.
+l: This indicates that you are setting a user limit.
limit: Replace this with the maximum number of users you want to allow in the channel.
*/