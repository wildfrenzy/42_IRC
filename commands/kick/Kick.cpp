/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmaliare <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/16 05:08:29 by nmaliare          #+#    #+#             */
/*   Updated: 2023/09/16 05:29:04 by nmaliare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Kick.hpp"
#include "./../../client/Client.hpp"
#include <string>

Kick::Kick() : Cmd(){}
Kick::~Kick() {}
Kick::Kick(const Kick &k) {
	(void)&k;
}
Kick &Kick::operator=(const Kick &k) {
	(void)&k;
	return *this;
}

bool Kick::isMember(std::vector<Client *> members, std::string nick) const{
	for (std::vector<Client*>::iterator it = members.begin() ; it != members.end() ; ++it) {
		if ((*it)->getNickName() == nick)
			return true;
	}
	return false;
}

std::string Kick::cmdToStr(std::vector<std::string> cmd) const{
	std::string str;
	for (size_t i = 3; i < cmd.size(); ++i) {
		str.append(cmd[i]);
		if (i != cmd.size() - 1)
			str.append(" ");
	}
	return str;
}

Client *findHim(std::vector<Client *> clients, std::string nick) {
	Client *cl = NULL;
	for (std::vector<Client*>::iterator it = clients.begin() ; it != clients.end() ; ++it) {
		if ((*it)->getNickName() == nick)
			return *it;
	}
	return cl;
}

Client *findNotHim(std::vector<Client *> clients, std::string nick) {
	Client *cl = NULL;
	for (std::vector<Client*>::iterator it = clients.begin() ; it != clients.end() ; ++it) {
		if ((*it)->getNickName() != nick)
			return *it;
	}
	return cl;
}

//   Parameters: <channel> <user> [<comment>]
void Kick::execute(Client &who, std::vector <std::string> cmd) const {
	Server *serv = who.getServer();
	std::map<std::string, Channel *> &channels = serv->getChannels();

	if (!who.getRegistered()) {
		serv->reply(&who, "ERR_NOTREGISTERED", ":You must authenticate with the server.");
		return;
	} else if (cmd.size() < 3) {
		serv->reply(&who, "ERR_NEEDMOREPARAMS", "KICK :Not enough parameters");
		return;
	} else if (channels.find(cmd[1]) == channels.end()){
		serv->reply(&who, "ERR_NOSUCHCHANNEL", cmd[1] + " :No such channel");
		return;
	} else if (!channels[cmd[1]]->operatorRight(who)){
		serv->reply(&who, "ERR_CHANOPRIVSNEEDED", cmd[2] + " " + cmd[1] + " :You're not channel operator");
		return;
	} else if (!isMember(channels[cmd[1]]->getMembers(), cmd[2])){
		serv->reply(&who, "ERR_USERNOTINCHANNEL", cmd[2] + " " + cmd[1] +
			" :They aren't on that channel");
		return;
	} else if (!isMember(channels[cmd[1]]->getMembers(), who.getNickName())){
		serv->reply(&who, "ERR_NOTONCHANNEL", cmd[2] + " " + cmd[1] + " :You aren't on that channel");
		return;
	}

	std::string reason = cmdToStr(cmd);
	if (reason.find(':') != std::string::npos)
		reason = reason.substr(reason.find(':') + 1);
	serv->reply(channels[cmd[1]]->getMembers(), "", "KICK " + cmd[1] + " " + cmd[2] + " " +
		(!reason.empty() ? reason : ":No reason specified" ));

	Client *found = findHim(channels[cmd[1]]->getMembers(), cmd[2]);
	if (found)
	{
		if (channels.size() > 1){
			Client *newOp = findNotHim(channels[cmd[1]]->getMembers(), cmd[2]);
			channels[cmd[1]]->addOperator(*newOp);
		}
		channels[cmd[1]]->deleteMembers(found);
		found = findHim(channels[cmd[1]]->getOperators(), cmd[2]);
		if (found)
			channels[cmd[1]]->deleteOperator(found);
		found = findHim(channels[cmd[1]]->getInvitee(), cmd[2]);
		if (found)
			channels[cmd[1]]->removeInvitee(found);
	}
	else
		std::cout << RED"not found"RES << std::endl;
}
