#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <set>
#include "../server/Server.hpp"
#include "../client/Client.hpp"

class Channel
{
    public:
        Channel(void);
        ~Channel(void){};
        Channel(const Channel& other);
        Channel& operator=(const Channel& other);
    
        // void    addChannel(Client& who, std::vector<std::string> operator);
        // void    deleteChannel(Client& who, std::vector<std::string> operator);
    
        // void    setKey(std::string key);
        // void    setTopic(std::string topic);
        void    setInviteOnly(bool invite_only);
        void    setUserLimit(size_t user_limit);
        std::string getKey(void);
        size_t  getUserLimit(void);
        bool    getInviteOnly(void);


    private:
        size_t  _user_limit;
        bool    _invite_only;
        std::string _key;
        std::string _topic;
        std::set<Client*> _members;
        std::set<Client*> _operators;
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