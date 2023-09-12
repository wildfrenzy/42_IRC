/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cmd.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yli <yli@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 15:33:39 by yli               #+#    #+#             */
/*   Updated: 2023/09/12 18:28:56 by yli              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cmd.hpp"
#include <string>
        // Cmd(const std::string & str, Client & c);
        // ~Cmd(void) {};
        // Cmd(const Cmd & other);
        // Cmd& operator=(const Cmd& other);

Cmd::Cmd(const std::string & str, Client & c): _client(c), _server(NULL), 
{
        cmdTokens(&str);
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
    this->_tokens = tokens;
    for (char c: tokens.front())
    {
        if (!std::isupper(c)) //just print "unknown command" message;
            throw std::invalid_argument("irc cmd: " + std::string(strerror(errno)));
    }
}