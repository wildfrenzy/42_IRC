#ifndef PRIVMSG_HPP
#define PRIVMSG_HPP

#include "./../../cmd/Cmd.hpp"
#include "./../../channel/Channel.hpp"
#include "./../../client/Client.hpp"
#include "./../topic/Topic.hpp"

class Client;
class Cmd;
class Channel;
class Mode;

class Privmsg: /*public Cmd, */public Mode
{
    public:
        Privmsg(void);
        ~Privmsg(void);
        Privmsg(const Privmsg& other);
        Privmsg& operator=(const Privmsg& other);
    
        void    sendToClient(Client& who, std::vector<std::string> cmd) const;
        void    sendToChannel(Client& who, std::vector<std::string> cmd) const;
        void    execute(Client& who, std::vector<std::string> cmd) const;
};




#endif