/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmaliare <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 18:00:49 by nmaliare          #+#    #+#             */
/*   Updated: 2023/09/24 19:10:24 by nmaliare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"
#include "./../../client/Client.hpp"

User::User() : Cmd(){}
User::~User() {}

#define YELLOW "\x1b[1;93m"
#define RES "\x1b[0m"

// USER <username> 0 * :Real name
void User::execute(Client &who, std::vector <std::string> cmd) const {
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
	return *this;
}
