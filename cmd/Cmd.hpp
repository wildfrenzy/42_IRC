/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cmd.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yli <yli@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 15:33:36 by yli               #+#    #+#             */
/*   Updated: 2023/09/12 19:18:55 by yli              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CMD_HPP
#define CMD_HPP

#include "../server/Server.hpp"
#include "../client/Client.hpp"

class Cmd
{
    public:
        Cmd(const std::string & str, Client & c);
        virtual ~Cmd(void) {};
        Cmd(const Cmd & other); //todo
        Cmd& operator=(const Cmd& other); //todo
        
        virtual void    execute(std::vector<std::string> tokens) = 0;
        Server  *get_server(void); 
        Client  &get_client(void);
        void    cmdTokens(std::string& input);
        
    
    protected:
        std::vector<std::string> _tokens;
        Server *_server;
        Client &_client;
};


#endif