/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cmd.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yli <yli@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 15:33:36 by yli               #+#    #+#             */
/*   Updated: 2023/09/28 17:27:30 by yli              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CMD_HPP
#define CMD_HPP

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
        
        virtual void	execute(Client &who, std::vector<std::string> tokens) const = 0;
        bool    cmdcheck(std::string cmd) const;
        bool    intcheck(std::string cmd) const;
        Channel* findChannel(Client& who, std::string channelName) const;
        Client* findClient(Client& who, std::string nickName) const;
	private:
	Cmd(const Cmd & other);
	Cmd& operator=(const Cmd& other);
};

#endif