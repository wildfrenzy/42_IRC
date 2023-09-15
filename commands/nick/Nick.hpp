#ifndef NICK_HPP
#define NICK_HPP

//#include "./../../client/Client.hpp"
#include "./../../cmd/Cmd.hpp"


class Cmd;
class Client;

class Nick: public Cmd
{
    public:
        Nick();
        ~Nick();
    
        void    execute(Client& who, std::vector<std::string> cmd) const;
    
    private:
        Nick(const Nick& other);
        Nick& operator=(const Nick& other);
};




#endif