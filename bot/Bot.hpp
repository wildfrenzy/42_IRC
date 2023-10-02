/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yli <yli@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 18:54:53 by yli               #+#    #+#             */
/*   Updated: 2023/10/02 17:46:19 by yli              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BOT_HPP
#define BOT_HPP

#include "./../cmd/Cmd.hpp"
#include <ctime>
#include <unistd.h>

class Cmd;
class Client;
class Channel;

class Bot : public Cmd
{
    public:
        Bot(void);
        ~Bot(void);
        Bot(const Bot& other);
        Bot&    operator=(const Bot& other);

        std::string    answerTime(void) const;
        std::string    drawanimals(std::string str) const;
        void    execute(Client& who, std::vector<std::string> cmd) const;
};

#endif

