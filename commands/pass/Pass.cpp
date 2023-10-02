/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmaliare <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 18:20:52 by nmaliare          #+#    #+#             */
/*   Updated: 2023/09/15 16:56:15 by yli              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Pass.hpp"
#include "./../../client/Client.hpp"

Pass::Pass() : Cmd() {}

Pass::~Pass() {}

Pass::Pass(const Pass &p) {
	*this = p;
}

Pass &Pass::operator=(const Pass &p) {
	(void) p;
	return *this;
}

void Pass::execute(Client &who, std::vector <std::string> cmd) const {

	if (who.getAuthenticated()){
		who.getServer()->reply(&who,
										 "ERR_ALREADYREGISTERED",
										 ":You may not reregister");
		return;
	}
	if (cmd.size() < 2) {
		who.getServer()->reply(&who,
							   "ERR_NEEDMOREPARAMS",
							   "PASS :Not enough parameters");
		return;
	}
	if (cmd.size() > 2 || cmd[1] != who.getServer()->getPass()) {
		who.getServer()->reply(&who,
							   "ERR_PASSWDMISMATCH",
							   ":Password incorrect");
		return;
	}
	who.setAuthenticated();
	if (!who.getRealName().empty() && !who.getNickName().empty() && !who.getUserName().empty())
		who.setRegistered();
}
