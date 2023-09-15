/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cmd.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yli <yli@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 15:33:36 by yli               #+#    #+#             */
/*   Updated: 2023/09/15 18:43:04 by yli              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CMD_HPP
#define CMD_HPP

#include "./../client/Client.hpp"
#include "./../server/Server.hpp"
#include <vector>
#include <string>
#include <map>

class Client;
class Channel;

class Cmd
{
    public:
		Cmd();
        virtual ~Cmd();
        Cmd(const Cmd & other);
        Cmd& operator=(const Cmd& other);
        
        virtual void	execute(Client &who, std::vector<std::string> tokens) const;
        bool    cmdcheck(std::string cmd) const;
        Channel* findChannel(Client& who, std::string channelName) const;
        Client& findClient(Client& who, std::string nickName) const;
};

#endif