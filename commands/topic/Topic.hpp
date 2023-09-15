#ifndef TOPIC_HPP
#define TOPIC_HPP

#include "./../../cmd/Cmd.hpp"
#include "./../../channel/Channel.hpp"
#include "./../../client/Client.hpp"
#include "./../mode/Mode.hpp"
#include <ctime>

class Client;
class Cmd;
class Channel;
class Mode;

class Topic: /*public Cmd, */public Mode
{
    public:
        Topic(void);
        ~Topic(void);
        Topic(const Topic& other);
        Topic&  operator=(const Topic& other);

        void    execute(Client& who, std::vector<std::string> cmd) const;
        void    setTopic(const std::string topic, Channel *c) const;
        bool    checkOperatorRight(Client& who, Channel *c) const;
};



#endif