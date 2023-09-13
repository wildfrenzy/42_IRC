/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmaliare <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 18:00:49 by nmaliare          #+#    #+#             */
/*   Updated: 2023/09/13 20:54:11 by nmaliare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"

User::User() : Cmd(){}
User::~User() {}

// can we have users with same username ?
// USER <username> 0 * :Real name
void User::execute(Client &who, std::vector <std::string> cmd) const {
	if (!who.getAuthenticated()) {
		who.getServer()->reply(&who,
							   "ERR_NOTREGISTERED",
							   ":You must authenticate with the server using PASS command first");
		return;
	}
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

	if (cmd[2] != "0" && cmd[3] != "*"){
		who.getServer()->reply(&who,
							   "ERR_NEEDMOREPARAMS",
							   "USER :Not enough parameters");
		return;
	}
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
	who.setRegistered();
}


User::User(const User &u) {
	*this = u;
}

User &User::operator=(const User &u) {
	this->_tokens = u._tokens;
	return  *this;
}