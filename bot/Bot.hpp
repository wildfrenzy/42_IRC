#ifndef BOT_HPP
#define BOT_HPP

#include "./../cmd/Cmd.hpp"
#include  <ctime>

class Cmd;
class Client;

class Bot : public Cmd
{
    public:
        Bot(void);
        ~Bot(void);
        Bot(const Bot& other);
        Bot&    operator=(const Bot& other);

        std::string    answerTime(void) const;
        std::string    drawanimals(std::string str) const;
        void    execute(Client &who, std::vector<std::string> cmd) const;    
};


#endif

