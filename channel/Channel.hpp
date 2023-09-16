#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <iostream>
#include <set>
#include "../client/Client.hpp"
//#include "./../bot/Bot.hpp"

class Client;
class Server;

class Channel
{
    public:
        Channel(void);
        ~Channel(void);


        void    setInviteOnly(bool invite_only);
        void    setUserLimit(size_t size);
        void    setChannelName(std::string& channelName);
        void    setKey(const std::string key);
        void    setTopic(std::string topic);
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
		void    deleteMembers(Client& c);
		void    deleteMembers(Client* c);
        void    addMember(Client& c);
		std::vector<Client*> &getMembers();
        void    broadcast(Server * server, std::string& msg);
        std::vector<Client*> &getOperators(void);

    private:
        std::string _channelName;
        size_t  _user_limit;
        bool    _invite_only;
        bool    _topic_right;
        std::string _key;
        std::string _topic;
        std::vector<Client*> _members;
        std::vector<Client*> _operators;
		Channel(const Channel& other);
		Channel &operator=(const Channel& other);
};


#endif

/*
Change the channel’s mode:

i: Set/remove Invite-only channel
t: Set/remove the restrictions of the TOPIC command to channel operators
k: Set/remove the channel key (password)
o: Give/take channel operator privilege
l: Set/remove the user limit to channel*/