/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Privmsg.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yli <yli@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 18:53:18 by yli               #+#    #+#             */
/*   Updated: 2023/09/20 21:22:08 by yli              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PRIVMSG_HPP
#define PRIVMSG_HPP

#include "./../../cmd/Cmd.hpp"
// #include "./../../channel/Channel.hpp"
// #include "./../../client/Client.hpp"

class Client;
class Cmd;
class Channel;

class Privmsg: public Cmd
{
    public:
        Privmsg(void);
        ~Privmsg(void);
    
        void    sendToClient(Client& who, std::vector<std::string> cmd) const;
        void    sendToChannel(Client& who, std::vector<std::string> cmd) const;
        void    execute(Client& who, std::vector<std::string> cmd) const;
	private:
		Privmsg(const Privmsg& other);
		Privmsg& operator=(const Privmsg& other);
};

#endif

//   :Angel PRIVMSG Wiz :Hello are you receiving this message ?
//                                   ; Message from Angel to Wiz.

//   :dan!~h@localhost PRIVMSG #coolpeople :Hi everyone!
//                                   ; Message from dan to the channel
//                                   #coolpeople