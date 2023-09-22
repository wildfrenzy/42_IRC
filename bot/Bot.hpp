/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yli <yli@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 18:54:53 by yli               #+#    #+#             */
/*   Updated: 2023/09/22 16:58:27 by yli              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BOT_HPP
#define BOT_HPP

#include "./../cmd/Cmd.hpp"
#include <ctime>
#include <exception>
#include <unistd.h>


class Cmd;
class Client;
class Channel;

class Bot
{
    public:
        Bot(void);
        ~Bot(void);
        Bot(const Bot& other);
        Bot&    operator=(const Bot& other);

        std::string    answerTime(void) const;
        std::string    drawanimals(std::string str) const;
        void    botexecute(Client &who, std::vector<std::string> cmd) const;
    //     Client* getClient(void);
    //     void    setClient(void);
    
    //     class PipeErrorException: std::exception {virtual const char* what() const throw() {return "Bot: PipeErrorException";}};
    // private:
    //     Client* _bot;
    //     int _pipe_fd;
};

#endif

