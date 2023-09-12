/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmaliare <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 18:20:52 by nmaliare          #+#    #+#             */
/*   Updated: 2023/09/12 18:20:52 by nmaliare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Pass.hpp"

Pass::Pass() {}

Pass::~Pass() {}

Pass::Pass(const Pass &p) { (void)p; }

Pass &Pass::operator=(const Pass &p) {
	(void)p;
	return *this;
}

void Pass::execute(std::vector <std::string> cmd) {
	if (this->_client.getAuthenticated())
		this->_reply(462);
	else if (cmd.size() < 2)
		this->_reply(461);
	else if (cmd.size() > 2)
		this->_reply(464);
}

std::string Pass::_reply(int err) {
	std::string errorMsg;
	//this->_client.getNickName()
	if (err == 462)
	{
		errorMsg = ":irc server 462 " + " clientNICK " + ":You may not reregister"
		this->_client.setWriteBuff(errorMsg);
	}
	else if (err == )
}