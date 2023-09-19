/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yli <yli@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 18:00:49 by nmaliare          #+#    #+#             */
/*   Updated: 2023/09/15 14:37:55 by yli              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"
#include "./../../client/Client.hpp"

User::User() : Cmd(){}
User::~User() {}

#define BLUE "\x1b[1;36m"
#define YELLOW "\x1b[1;93m"
#define RED "\x1b[1;31m"
#define RES "\x1b[0m"

// can we have users with same username ?
// USER <username> 0 * :Real name
void User::execute(Client &who, std::vector <std::string> cmd) const {
	/*if (!who.getAuthenticated()) {
		who.getServer()->reply(&who,
							   "ERR_NOTREGISTERED",
							   ":You must authenticate with the server.");
		return;
	}*/
	if (who.getRegistered()) {
		who.getServer()->reply(&who,
							   "ERR_ALREADYREGISTERED",
							   ":You may not reregister");
		return;
	}
	if (cmd.size() < 5){
		who.getServer()->reply(&who,
							   "ERR_NEEDMOREPARAMS",
							   "USER :Not enough parameters");
		return;
	}
	//This error code is used to indicate that
	// a client's command is missing required parameters or is incorrectly formatted.
	//todo: add better validation of arguments
	who.setUserName(cmd[1]);
	std::string rn;
	if (cmd[4][0] != ':')
		who.setRealName(cmd[4]);
	else
	{
		cmd[4] = cmd[4].substr(1);
		for (size_t i = 4; i < cmd.size(); ++i) {
			rn.append(cmd[i] + " ");
		}
		who.setRealName(rn);
	}
	who.getServer()->reply(&who, "", YELLOW"[" + who.getUserName() + "]" + " Username set successfully." + RES);
	if (who.getAuthenticated() && !who.getRegistered() && !who.getNickName().empty())
		who.setRegistered();
}


User::User(const User &u) {
	*this = u;
}

User &User::operator=(const User &u) {
	(void)u;
	return  *this;
}
