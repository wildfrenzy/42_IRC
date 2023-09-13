#ifndef PRIVMSG_HPP
#define PRIVMSG_HPP

#include "./../../cmd/Cmd.hpp"
#include "./../../channel/Channel.hpp"
#include "./../../client/Client.hpp"

class Cmd;
class Client;
class Channel;

class Privmsg: public Cmd, public Channel
{
    public:
        Privmsg(void){};
        ~Privmsg(void){};
    
        void    execute(Client& who, std::vector<std::string> cmd) const;
    
    private:
        //std::string _reply(int err);
        Privmsg(const Privmsg& other);
        Privmsg& operator=(const Privmsg& other);
};




#endif


// class Cmd;
// class Client;

// class Nick: public Cmd
// {
//     public:
//         Nick(void){};
//         ~Nick(void){};
    
//         void    execute(Client& who, std::vector<std::string> cmd) const;
    
//     private:
//         std::string _reply(int err);
//         Nick(const Nick& other);
//         Nick& operator=(const Nick& other);
// };