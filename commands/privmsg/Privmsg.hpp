#ifndef PRIVMSG_HPP
#define PRIVMSG_HPP

#include "./../../cmd/Cmd.hpp"
// #include "./../../channel/Channel.hpp"
// #include "./../../client/Client.hpp"

class Client;
class Cmd;
class Channel;

class Privmsg: public Cmd
{
    public:
        Privmsg(void);
        ~Privmsg(void);
    
        void    sendToClient(Client& who, std::vector<std::string> cmd) const;
        void    sendToChannel(Client& who, std::vector<std::string> cmd) const;
        void    execute(Client& who, std::vector<std::string> cmd) const;
	private:
		Privmsg(const Privmsg& other);
		Privmsg& operator=(const Privmsg& other);
};




#endif