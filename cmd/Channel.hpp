#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "../server/Server.hpp"
#include "../client/Client.hpp"

class Channel
{
    public:
        Channel(void);
        ~Channel(void);
        Channel(const Channel& other);
        Channel& operator=(const Channel& other);
    
    private:
        size_t  maxsize;
        bool    invite_only;
        std::string _key;
        std::string _topic;

}


#endif