/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cmd.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yli <yli@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 15:33:39 by yli               #+#    #+#             */
/*   Updated: 2023/09/13 18:09:03 by yli              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cmd.hpp"

Cmd::Cmd() {}

Cmd::~Cmd() {}

Cmd::Cmd(const Cmd &other) {
	*this = other;
}

Cmd &Cmd::operator=(const Cmd &other) {
	this->_tokens = other._tokens;
	return *this;
}

/*

void    PRIVMSG(std::vector<std::string> tokens)
{
    if (tokens.size() <= 2)
        throw std::invalid_argument("irc cmd PRIVMSG: " + std::string(strerror(errno)));
    char c = tokens[1][0];
    if (c == '#'){}
        //we dont have channel now
    else
    {
        std::vector<Client *>::iterator it;
        for(it = this->_server.getClients().begin(); it != this->_server.getClients().end(); ++it)
        {
            if ((*it)->getNickName() == tokens[1])
            {
                if (tokens[2][0] == ':')
                {
                    for (size_t i = 2; i < tokens.size(); ++i)
                        (*it)->setWriteBuff(tokens[i]); 
                }
                else
                {
                    std::string error = (*it)->getHost() + " 400 " + this->_client.getNickName() + " :Could not process multiple invalid parameters";
                    (*it)->setWriteBuff(error);
                }
            }
            else
            {
                std::string error = (*it)->getHost() + " 401 " + this->_client.getNickName() + " " + tokens[1] + " :No such nick/channel";
                (*it)->setWriteBuff(error);
            }
        }
    }
}*/
