#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <set>
#include "../server/Server.hpp"
#include "../client/Client.hpp"

class Client;

class Channel
{
    public:
        Channel(void);
        ~Channel(void){};
        Channel(const Channel& other);
        Channel& operator=(const Channel& other);

        void    setInviteOnly(bool invite_only);
        void    setUserLimit(size_t size);
        void    setChannelName(std::string& channelName);
        void    setKey(const std::string key);
        void    setTopic(const std::string topic);
        void    setTopicRight(bool topic_right);
        std::string getKey(void);
        size_t  getUserLimit(void);
        bool    getInviteOnly(void);
        std::string getChannelName(void);
        bool    operatorRight(Client& c);
        size_t  getMemberSize(void);
        bool    getTopicRight(void);
        std::string getTopic(void);
        void    addOperator(Client& c);
        void    deleteOperator(Client& c);
        void    addMember(Client& c);
		std::vector<Client*> &getMembers();
        void    broadcast(std::string& msg);

    private:
        std::string _channelName;
        size_t  _user_limit;
        bool    _invite_only;
        bool    _topic_right;
        const std::string _key;
        const std::string _topic;
        std::vector<Client*> _members;
        std::vector<Client*> _operators;
};


#endif

/*Set: Set is also one of the templates of Standard Template Library or STL. 
It is a container of unique elements whose value can’t be modified once added to the set, but can be deleted or inserted. 
The elements of the sets are always stored in sorted form.*/

/*
Change the channel’s mode:

i: Set/remove Invite-only channel
t: Set/remove the restrictions of the TOPIC command to channel operators
k: Set/remove the channel key (password)
o: Give/take channel operator privilege
l: Set/remove the user limit to channel*/