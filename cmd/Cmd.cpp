/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cmd.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yli <yli@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 15:33:39 by yli               #+#    #+#             */
/*   Updated: 2023/09/12 21:38:20 by yli              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cmd.hpp"

Cmd::Cmd(const std::string & str, Client & c): _client(c), _server(NULL), 
{
        cmdTokens(str);
}

Cmd::~Cmd() {}

void    Cmd::cmdTokens(std::string& input)
{
    std::string deli = " ";
    std::vector<std::string> tokens;
    std::string token;
    size_t pos = 0;
    input += " ";
    pos = input.find(deli);
    int i = 0;
    while (pos != std::string::npos)
    {
        token = input.substr(0, pos);
        tokens.push_back(token);
        input.erase(0, pos + deli.size());
        pos = input.find(deli);
    }
    if (tokens.empty())
        throw std::invalid_argument("irc cmd: " + std::string(strerror(errno)));
    this->_tokens = tokens;
    const std::string& firstToken = tokens.front();
    for (size_t i = 0; i < firstToken.length(); ++i)
    {
        char c = firstToken[i];
        if (!std::isupper(c)) {
            throw std::invalid_argument("irc cmd: " + std::string(strerror(errno)));
        }
    }
}

Server* Cmd::get_server(void)
{
    return this->_server;
}

Client & Cmd::get_client(void)
{
    return this->_client;
}

void    Cmd::set_server(Server *s)
{
    this->_server = s;
}


void    Cmd::NICK(std::vector<std::string> tokens)
{
    if (tokens.size() != 2)
        throw std::invalid_argument("irc cmd NICK: " + std::string(strerror(errno)));
    std::string ni = tokens[1];
    std::vector<Client *>::iterator it;
    for(it = this->_server.getClients().begin(); it != this->_server.getClients().end(); ++it)
    {
        if ((*it)->getNickName() == ni)
            throw std::invalid_argument("irc cmd NICK: " + std::string(strerror(errno)));
    }
    this->_client.setNickName(ni);
}

void    PRIVMSG(std::vector<std::string> tokens)
{
    if (tokens.size() <= 2)
        throw std::invalid_argument("irc cmd PRIVMSG: " + std::string(strerror(errno)));
    char c = tokens[1][0];
    if (c == '#')
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
}