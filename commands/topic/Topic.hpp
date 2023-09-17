#ifndef TOPIC_HPP
#define TOPIC_HPP

#include "./../../cmd/Cmd.hpp"
// #include "./../../channel/Channel.hpp"
// #include "./../../client/Client.hpp"
#include <ctime>

class Client;
class Cmd;
class Channel;

class Topic: public Cmd
{
    public:
        Topic(void);
        ~Topic(void);

        void    execute(Client& who, std::vector<std::string> cmd) const;
        void    setTopic(std::string& topic, Channel *c) const;
        bool    checkOperatorRight(Client& who, Channel *c) const;
	private:
		Topic(const Topic& other);
		Topic&  operator=(const Topic& other);
};



#endif