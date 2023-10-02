/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yli <yli@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 01:07:17 by yli               #+#    #+#             */
/*   Updated: 2023/09/24 23:46:56 by nmaliare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Nick.hpp"
#include "./../../client/Client.hpp"
Nick::Nick() : Cmd() {}

Nick::~Nick() {}

Nick::Nick(const Nick& other)
{
	*this = other;
}

Nick& Nick::operator=(const Nick& other)
{
	(void)other;
	return *this;
}

void Nick::execute(Client& who, std::vector<std::string> cmd) const
{
	if (cmd.size() < 2) {
		who.getServer()->reply(&who,
							   "ERR_NEEDMOREPARAMS",
							   ":Not enough parameters");
		return;
	}
	std::string ni = cmd[1];
	for(unsigned long i = 0; i < ni.size(); ++i)
	{
		if (!std::isalnum(ni[i]) && ni[i] != '_'){
			who.getServer()->reply(&who, "ERR_ERRONEUSNICKNAME", ":Erroneous nickname");
			return;
		}
	}
	for(unsigned long i = 0 ; i < who.getServer()->getClients().size(); ++i)
	{
		if (who.getServer()->getClients()[i]->getNickName() == ni){
			size_t num = who.getServer()->getClients().size();
			std::stringstream str;
			str << num;
			std::string result;
			str >> result;
			who.getServer()->replyNoServ(&who, ":irc_serv 433 " + ni + " user" + result + " :Nickname is already in use");
			return;
		}
	}
	if (!who.getNickName().empty()){
		who.getServer()->replyNoServ(&who, ":" + who.getNickName() + "!" + who.getUserName() + "@" + who.getHost() +
			" NICK " + ni);
	} else
		who.getServer()->reply(&who, "", YELLOW"[" + ni  + "]" + " Nickname set successfully." + RES);

	who.setNickName(ni);
	if (who.getAuthenticated() && !who.getRegistered() && !who.getUserName().empty())
		who.setRegistered();
}

/*In IRC (Internet Relay Chat), server implementations may have their own specific rules and restrictions regarding channel and nickname naming, but generally, 
the following characters are commonly disallowed or restricted:
Space: Spaces are not allowed in channel names, nicknames, or usernames. 
Channels and nicks are usually separated by spaces, so including a space in any of them would cause parsing issues.

Control Characters: Control characters, such as ASCII control characters (e.g., ASCII 0-31), 
are typically not allowed in channel names, nicknames, or usernames. 
These characters are often used for special control functions in IRC clients and servers.

Special IRC Characters: Some IRC networks disallow certain special characters in channel names or nicknames, 
like: #, &, +, !, @, and %, as these characters often have special meanings in IRC.

Non-Printable Characters: Non-printable characters, including some Unicode characters and emojis, 
may be restricted or filtered by IRC servers to prevent misuse or abuse.

Punctuation: Some IRC servers may disallow certain punctuation marks or symbols, 
such as slashes (/), backslashes (), asterisks (*), and question marks (?), to prevent potential command injection or confusion.

Extended Characters: Extended or non-ASCII characters may not be supported universally 
or may be restricted in IRC names depending on the server's configuration and character encoding settings.*/
