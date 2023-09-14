#ifndef MODE_HPP
#define MODE_HPP

#include "./../../cmd/Cmd.hpp"
#include "./../../channel/Channel.hpp"
#include "./../../client/Client.hpp"

class Client;
class Cmd;
class Channel;


class   Mode: public Cmd
{
    public:
        Mode(void);
        ~Mode(void);
        Mode(const Mode& other);
        Mode& operator=(const Mode& other);
    
        void    execute(Client& who, std::vector<std::string> cmd) const;
        Channel* findChannel(Client& who, std::string channelName);
        Client& findClient(Client& who, std::string nickName)

        void    setInviteOnly(void);
        void    unsetInviteOnly(void);
        void    setKey(const std::string key);
        void    unsetKey(void);
        void    setTopicRight(void);
        void    unsetTopicRight(void);
        void    setUserLimit(size_t size);
        void    unsetUserLimit(void);
        void    setUserPrivilege(Client& c);
        void    unsetUserPrivileg(Client& c);
        void    plusmode(Client& who, std::vector<std::string> cmd);
        void    minusmode(Client& who, std::vector<std::string> cmd);
        bool    thirdcmdcheck(std::string content);

    private:
        Channel*    _channel;
}


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