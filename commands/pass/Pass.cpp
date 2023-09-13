/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmaliare <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 18:20:52 by nmaliare          #+#    #+#             */
/*   Updated: 2023/09/13 04:43:25 by nmaliare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Pass.hpp"

Pass::Pass() : Cmd() {}

Pass::~Pass() {}

Pass::Pass(const Pass &p) {
	*this = p;
}

Pass &Pass::operator=(const Pass &p) {
	this->_tokens = p._tokens;
	return *this;
}

void Pass::execute(Client &who, std::vector <std::string> cmd) const {

/*	std::cout << "VEC CHECK: ";
	for (int i = 0; i < cmd.size(); ++i) {
		std::cout << "[" << cmd[i] << "]";
	}
	std::cout << std::endl;*/

	if (who.getAuthenticated()){
		who.getServer()->reply(&who,
										 "ERR_ALREADYREGISTERED",
										 ":You may not reregister");
		return;
	}
	if (cmd.size() < 2) {
		who.getServer()->reply(&who,
							   "ERR_NEEDMOREPARAMS",
							   ":Not enough parameters");
		return;
	}
	if (cmd.size() > 2 || cmd[1] != who.getServer()->getPass()) {
		who.getServer()->reply(&who,
							   "ERR_PASSWDMISMATCH",
							   ":Password incorrect");
		return;
	}
	who.setAuthenticated();
}
